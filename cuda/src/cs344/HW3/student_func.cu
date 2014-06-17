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


template <class Type> __device__ Type min_cuda( Type a, Type b ) {
  // I - +inf
  return a < b ? a : b;
}

template <class Type> __device__ Type max_cuda( Type a, Type b ) {
  // I - -inf
  return a > b ? a : b;
}

inline int isPow2(int a) {
  return !(a&(a-1));
}

class ReduceOperation {
public:
  virtual ~ReduceOperation() {}
  __device__ 
  virtual float operator()(float a, float b) const = 0;
  __device__
  virtual float I() const = 0;
};

class ComparatorMax : public ReduceOperation {
public:
  __device__ 
  virtual float operator()(float a, float b) const {
    return max_cuda<float>(a, b);
  }
  
  ComparatorMax() : I_val(-FLT_MAX) {}
  //explicit ComparatorMax(float value) : I_val(value) {}
  
  __device__
  virtual float I() const {
    return I_val;
  }
private:
  const float I_val;
};

__global__ void shmem_max_reduce_kernel(
    float * d_out, 
    const float * d_in /*для задания важна константность*/,
    const int size/*, const ReduceOperation* const op*/)
{
    // sdata is allocated in the kernel call: 3rd arg to <<<b, t, shmem>>>
    extern __shared__ float sdata[];
    
    //op->I();  // no way
    
    //float I = 
    

    int myId = threadIdx.x + blockDim.x * blockIdx.x;
    int tid  = threadIdx.x;

    // load shared mem from global mem
    if (myId < size)
      sdata[tid] = d_in[myId];
    else {
      // заполняем нейтральными элементами
      sdata[tid] = 
      //op->I();  // no way
      -FLT_MAX;
    }
    __syncthreads();            // make sure entire block is loaded!
    
    // do reduction in shared mem
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s)
        {
          float tmp =  
	    max_cuda<float>
	    //(*op)
	    (sdata[tid], sdata[tid + s]); 
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

void reduce_shared(float const * const d_in, float * const d_out, int size, const ReduceOperation* const op) 
{
  int threads = maxThreadsPerBlock;
  int blocks = ceil((1.0f*size) / maxThreadsPerBlock);
  int ARRAY_BYTES = size * sizeof(float);
  
  // assumes that size is not greater than maxThreadsPerBlock^2
  // and that size is a multiple of maxThreadsPerBlock
  assert(size <= threads * threads);  // для двушаговой редукции, чтобы уложиться
  assert(blocks * threads >= size);  // нужно будет ослабить - shared-mem дозаполним внутри ядер
  assert(isPow2(threads));  // должно делиться на 2 до конца
  
  float * d_intermediate;  // stage 1 result
  cudaMalloc((void **) &d_intermediate, ARRAY_BYTES); // overallocated

  // Step 1: Вычисляем результаты для каждого блока
  // TODO: Error!!! "Segfault"
  shmem_max_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_intermediate, d_in, size/*, op*/);
  cudaDeviceSynchronize(); 
  checkCudaErrors(cudaGetLastError());

  // Step 2: Комбинируем разультаты блоков и это ограничение на размер входных данных
  // now we're down to one block left, so reduce it
  threads = blocks; // launch one thread for each block in prev step
  blocks = 1;
  //shmem_max_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_out, d_intermediate, threads, op);
  
  cudaFree(d_intermediate);
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
  
  float* d_elem;
  float h_elem;
  cudaMalloc((void **) &d_elem, sizeof(float));  // 1 значение
  
  ComparatorMax op;
  reduce_shared(d_logLuminance, d_elem, numRows * numCols, &op);
  
  cudaFree(d_elem);
  
  /*
    2) subtract them to find the range
    
    // Похоже гистограмма как таковая не нужна
    // TODO: Можно ли использовать cdf? кажется можно
    3) generate a histogram of all the values in the logLuminance channel using
       the formula: bin = (lum[i] - lumMin) / lumRange * numBins
    4) Perform an exclusive scan (prefix sum) on the histogram to get
       the cumulative distribution of luminance values (this should go in the
       incoming d_cdf pointer which already has been allocated for you)       */


}
