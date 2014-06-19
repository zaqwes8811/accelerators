/* Udacity Homework 3
   HDR Tone-mapping

  Background HDR
  ==============

  A High Definition Range (HDR) image contains a wider variation of intensity
  and color than is allowed by the RGB format with 1 byte per channel that we
  have used in the previous assignment.  

  To store this extra information we use single precision floating point for
  each channel.  This allows for an extremely wide range of intensity values.

  In the image for this assignment, the inside of church with light coming in
  through stained glass windows, the raw input floating point values for the
  channels range from 0 to 275.  But the mean is .41 and 98% of the values are
  less than 3!  This means that certain areas (the windows) are extremely bright
  compared to everywhere else.  If we linearly map this [0-275] range into the
  [0-255] range that we have been using then most values will be mapped to zero!
  The only thing we will be able to see are the very brightest areas - the
  windows - everything else will appear pitch black.

  The problem is that although we have cameras capable of recording the wide
  range of intensity that exists in the real world our monitors are not capable
  of displaying them.  Our eyes are also quite capable of observing a much wider
  range of intensities than our image formats / monitors are capable of
  displaying.

  Tone-mapping is a process that transforms the intensities in the image so that
  the brightest values aren't nearly so far away from the mean.  That way when
  we transform the values into [0-255] we can actually see the entire image.
  There are many ways to perform this process and it is as much an art as a
  science - there is no single "right" answer.  In this homework we will
  implement one possible technique.

  Background Chrominance-Luminance
  ================================

  The RGB space that we have been using to represent images can be thought of as
  one possible set of axes spanning a three dimensional space of color.  We
  sometimes choose other axes to represent this space because they make certain
  operations more convenient.

  Another possible way of representing a color image is to separate the color
  information (chromaticity) from the brightness information.  There are
  multiple different methods for doing this - a common one during the analog
  television days was known as Chrominance-Luminance or YUV.

  We choose to represent the image in this way so that we can remap only the
  intensity channel and then recombine the new intensity values with the color
  information to form the final image.

  Old TV signals used to be transmitted in this way so that black & white
  televisions could display the luminance channel while color televisions would
  display all three of the channels.
  

  Tone-mapping
  ============

  In this assignment we are going to transform the luminance channel (actually
  the log of the luminance, but this is unimportant for the parts of the
  algorithm that you will be implementing) by compressing its range to [0, 1].
  To do this we need the cumulative distribution of the luminance values.

  Example
  -------

  input : [2 4 3 3 1 7 4 5 7 0 9 4 3 2]
  min / max / range: 0 / 9 / 9

  histo with 3 bins: [4 7 3]

  cdf : [4 11 14]


  Your task is to calculate this cumulative distribution by following these
  steps.

*/

// C
#include <float.h>
#include <stdio.h>

// reuse
#include "utils.h"

const int maxThreadsPerBlock = 1024;

template <class Type> __device__ __host__ Type cudaMin2( Type a, Type b ) {
  // I - +inf
  return a < b ? a : b;
}

template <class Type> __device__ __host__ Type cudaMax2( Type a, Type b ) {
  // I - -inf
  return a > b ? a : b;
}

inline __device__ __host__ int isPow2(int a) {
  return !(a&(a-1));
}

__global__ void min_max_reduce_kernel(
    const float * const d_in, float * const d_out, const int size, 
    const int key=0)
{
  assert(key < 2);
  assert(isPow2(blockDim.x || 0));  // должно делиться на 2 до конца
  
  //
  extern __shared__ float sdata[];

  float I_elem = -FLT_MAX;
  float (* op)(float, float)(cudaMax2);
  if (1 == key) 
  {
    I_elem = +FLT_MAX;
    op = cudaMin2;
  }
  
  // Reuse code
  int globalId = threadIdx.x + blockDim.x * blockIdx.x;
  int tid  = threadIdx.x;

  // load shared mem from global mem
  if (globalId < size)
    sdata[tid] = d_in[globalId];
  else 
  {
    sdata[tid] = I_elem;
  }
  __syncthreads();            // make sure entire block is loaded!
  
  // do reduction in shared mem
  for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
  {
      if (tid < s)
      {
	float tmp = (*op)(sdata[tid], sdata[tid + s]); 
	sdata[tid] = tmp;
      }
      __syncthreads();        // make sure all adds at one stage are done!
  }

  // only thread 0 writes result for this block back to global mem
  if (tid == 0)
  {
      d_out[blockIdx.x] = sdata[0];
  }
}

void reduce_shared(
    float const * const d_in, float * const d_out, int size, 
    const int key=0)// const ReduceOperation* const op) 
{
  int threads = maxThreadsPerBlock;
  int blocks = ceil((1.0f*size) / maxThreadsPerBlock);

  // assumes that size is not greater than maxThreadsPerBlock^2
  // and that size is a multiple of maxThreadsPerBlock
  assert(size <= threads * threads);  // для двушаговой редукции, чтобы уложиться
  assert(blocks * threads >= size);  // нужно будет ослабить - shared-mem дозаполним внутри блоков
  assert(isPow2(threads));  // должно делиться на 2 до конца
  
  float * d_intermediate;  // stage 1 result
  int ARRAY_BYTES = size * sizeof(float);
  cudaMalloc((void **) &d_intermediate, ARRAY_BYTES); // overallocated

  // Step 1: Вычисляем результаты для каждого блока
  min_max_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_in, d_intermediate, size, key);

  // Step 2: Комбинируем разультаты блоков и это ограничение на размер входных данных
  // now we're down to one block left, so reduce it
  threads = blocks; // launch one thread for each block in prev step
  blocks = 1;
  min_max_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_intermediate, d_out, threads, key);
  
  cudaFree(d_intermediate);
}

__global__ void simple_histo_kernel(
    const float * const d_logLuminance, const int size,
    unsigned int * const d_histo, const int numBins,
    float min_logLum, float logLumRange)
{ 
  int globalId = threadIdx.x + blockDim.x * blockIdx.x;
  if (globalId >= size)
    return; 
    
  float value = d_logLuminance[globalId];

  // bin
  unsigned int bin = cudaMin2(
      static_cast<unsigned int>(numBins - 1), 
      static_cast<unsigned int>((value - min_logLum) / logLumRange * numBins));

  // Inc global memory. Partial histos not used.
  atomicAdd(&(d_histo[bin]), 1);
}

__global__ void kern_exclusive_scan_cache(const unsigned int * const d_in, unsigned int * const d_out,    //float * const d_sink, 
    int n)
{ 
  // результаты работы потоков можем расшаривать через эту
  // память или через глобальную
  extern __shared__ unsigned int temp[]; 
  int globalId = threadIdx.x + blockDim.x * blockIdx.x;
  int localId  = threadIdx.x;
  
  // Load input into shared memory.  
  // This is exclusive scan, so shift right by one  
  // and set first element to 0  
  unsigned int tmpVal = 0;
  if (localId > 0)
    if (globalId < n)
    {
      tmpVal = d_in[globalId-1];
    }
    else 
      tmpVal = 0;

  temp[localId] = tmpVal;
  
  __syncthreads();  

  for (int offset = 1; offset < n; offset *= 2)  // 2^i
  {  
    if (localId >= offset) {
      unsigned int temp_val0 = temp[localId];
      unsigned int temp_val1 = temp[localId-offset]; 
      // TODO: возможно быстрее прибавить тут, а может и нет
      __syncthreads();
      
      temp[localId] = temp_val0 + temp_val1;  
    }
    
    // буффера переписали
    __syncthreads();  
  }  
  d_out[globalId] = temp[localId]; // write output 
}

// not float
__global__ void spread(const unsigned int* const d_tmp, unsigned int * const d_io, const int size) {
  int globalIdx = threadIdx.x + blockDim.x * blockIdx.x;
  if (globalIdx < size)
    d_io[globalIdx] += d_tmp[blockIdx.x];
}

__global__ void yeild_tailes(const unsigned int* const d_source, const unsigned int* const d_first_stage, unsigned int * d_out, const int size) {
  int globalIdx = threadIdx.x + blockDim.x * blockIdx.x;
  int tail_idx = blockDim.x * (blockIdx.x+1) - 1;
  if (globalIdx < size)
    d_out[blockIdx.x] = d_source[tail_idx] + d_first_stage[tail_idx];
}

void scan_hillis_single_block(const unsigned int * const d_in, unsigned int * const d_out, const int size) 
{
  
  int threads = maxThreadsPerBlock;
  int blocks = ceil((1.0f*size) / maxThreadsPerBlock);
  
  // assumes that size is not greater than maxThreadsPerBlock^2
  // and that size is a multiple of maxThreadsPerBlock
  assert(size <= threads * threads);  // для двушаговой редукции, чтобы уложиться
  assert(blocks * threads >= size);  // нужно будет ослабить - shared-mem дозаполним внутри ядер
  assert(isPow2(threads));  // должно делиться на 2 до конца - А нужно ли?
  //assert(blocks == 2);  // пока чтобы не комбинировать результаты блоков

  // Sectioned scan
  kern_exclusive_scan_cache<<< blocks, threads, threads * sizeof(unsigned int) >>>(d_in, d_out, size);
  cudaDeviceSynchronize();
  checkCudaErrors(cudaGetLastError());
  
  unsigned int * d_sink;
  unsigned int * d_sink_out;
  checkCudaErrors(cudaMalloc((void **) &d_sink, blocks * sizeof(unsigned int)));
  checkCudaErrors(cudaMalloc((void **) &d_sink_out, blocks * sizeof(unsigned int)));
  // map
  yeild_tailes<<< blocks, threads >>>(d_in, d_out, d_sink, size);
  cudaDeviceSynchronize();
  checkCudaErrors(cudaGetLastError());
  
  // запускаем scan на временном массиве
  kern_exclusive_scan_cache<<< 1, threads, threads * sizeof(unsigned int) >>>(d_sink, d_sink_out, threads);
  cudaDeviceSynchronize();
  checkCudaErrors(cudaGetLastError());
  
  // делаем map на исходном массиве
  spread<<< blocks, threads >>>(d_sink_out, d_out, size);
  cudaDeviceSynchronize();
  checkCudaErrors(cudaGetLastError());
  
  checkCudaErrors(cudaFree(d_sink));
  checkCudaErrors(cudaFree(d_sink_out));
}


// TODO: нужны временные буфферы
void your_histogram_and_prefixsum(const float* const d_logLuminance,
                                  unsigned int* const d_cdf,
                                  float &min_logLum,
                                  float &max_logLum,
                                  const size_t numRows,
                                  const size_t numCols,
                                  const size_t numBins)
{
 
  //TODO
  /*Here are the steps you need to implement
  1) find the minimum and maximum value in the input logLuminance channel
      store in min_logLum and max_logLum
      
      массив с данными должен быть не изменным, поэтому нужно хранить копию в shared
  */
  int size = numRows * numCols;
  
  float* d_elem;
  cudaMalloc((void **) &d_elem, sizeof(float));  // 1 значение
  reduce_shared(d_logLuminance, d_elem, size);
  cudaMemcpy(&max_logLum, d_elem, sizeof(float), cudaMemcpyDeviceToHost);
  
  // min
  int key = 1;
  reduce_shared(d_logLuminance, d_elem, size, key);
  cudaMemcpy(&min_logLum, d_elem, sizeof(float), cudaMemcpyDeviceToHost);
  cudaFree(d_elem);
  
  /*
    2) subtract them to find the range
  */
  float logLumRange = max_logLum - min_logLum;
  
  /*
  // Похоже гистограмма как таковая не нужна
  // TODO: Можно ли использовать cdf? кажется можно
  3) generate a histogram of all the values in the logLuminance channel using
      the formula: bin = (lum[i] - lumMin) / lumRange * numBins
  */
  unsigned int *d_histo;
  cudaMalloc((void **)& d_histo, numBins * sizeof(unsigned int));
  
  unsigned int h_histo[numBins];
  for(int i = 0; i < numBins; i++) h_histo[i] = 0;
  cudaMemcpy(d_histo, h_histo, numBins * sizeof(unsigned int), cudaMemcpyHostToDevice);
  
  int threads = maxThreadsPerBlock;
  int blocks = ceil((1.0f*size) / maxThreadsPerBlock);
  simple_histo_kernel<<< blocks, threads >>>(d_logLuminance, size, d_histo, numBins, min_logLum, logLumRange);
  

  /*
  4) Perform an exclusive scan (prefix sum) on the histogram to get
      the cumulative distribution of luminance values (this should go in the
      incoming d_cdf pointer which already has been allocated for you)       */
  
  scan_hillis_single_block(d_histo, d_cdf, numBins);

  cudaFree(d_histo);
}


