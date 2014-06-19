// TODO: расширить на несколько блоков
// Scan: 
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch39.html

// C
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// C++
#include <iostream>
#include <vector>
#include <algorithm>

// 3rdparty
#include <cuda_runtime.h>

// App
#include "float_ops.h"

#define checkCudaErrors(val) check( (val), #val, __FILE__, __LINE__)

template<typename T>
void check(T err, const char* const func, const char* const file, const int line) {
  if (err != cudaSuccess) {
    std::cerr << "CUDA error at: " << file << ":" << line << std::endl;
    std::cerr << cudaGetErrorString(err) << " " << func << std::endl;
    //assert(false && "CUDA error");
    exit(1);
  }
}

const int maxThreadsPerBlock = 1024;

using std::vector;
using std::equal;
using std::for_each;

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
  bool equal_adapt( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred )
{
  while (first1!=last1) {
    //if (!(*first1 == *first2))   // or: if (!pred(*first1,*first2)), for version 2
    //  return false;
    pred(*first1, *first2);
    ++first1; ++first2;
  }
  return true;
}

/*
// serial:
// TODO: причем тут f(elem)?
{
  out[0] = 0;
  for j from 1 to n do
    out[j] = out[j-1] + f(in[j-1]);
}
*/

/*
// Hillis and Steele
// parallel with one buffer:
// TODO: не понял в чем проблема, но похоже она в синхронизации
//   хотя нет, похоже дело в том что расчет in-place. Нет не в этом дело.
//   На стадиях обработки данные затираются.
for d = 1 to log2(n) do
  for all k in parallel do
    if k >= 2^d then
      x[k] = x[k - 2^(d-1)] + x[k]

// parallel separated in and out buffers:
for d = 1 to log2(n) do
  for all k in parallel do
    if k >= 2^d then
      x[out][k] = x[in][k-2^(d-1)] + x[in][k]
    else
      x[out][k] = x[in][k]
*/

// http://www.cplusplus.com/reference/algorithm/swap/
template <class T>
__device__ void cudaSwap(T& a, T& b) 
{
  int c(a); a=b; b=c;
}

// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch39.html
// TODO: In article finded bugs.
// DANGER: похоже слишком много синхронизации, возможно с двумя буфферами быстрее.
// TODO: убрать sink. лучше сделать еще один map
//template <class U>
__global__ void kern_exclusive_scan_cache(const float * const d_in, float * const d_out,    //float * const d_sink, 
    int n)
{ 
  // результаты работы потоков можем расшаривать через эту
  // память или через глобальную
  extern __shared__ float temp[]; 
  int globalId = threadIdx.x + blockDim.x * blockIdx.x;
  int localId  = threadIdx.x;
  
  // Load input into shared memory.  
  // This is exclusive scan, so shift right by one  
  // and set first element to 0  
  float tmpVal = 0;
  if (localId > 0)
    if (globalId < n)
      tmpVal = d_in[globalId-1];
    else 
      tmpVal = 0;

  temp[localId] = tmpVal;
  __syncthreads();  

  for (int offset = 1; offset < n; offset *= 2)  // 2^i
  {  
    if (localId >= offset) {
      float temp_val0 = temp[localId];
      float temp_val1 = temp[localId-offset]; 
      // TODO: возможно быстрее прибавить тут, а может и нет
      __syncthreads();
      
      temp[localId] = temp_val0 + temp_val1;  
    }
    
    // буффера переписали
    __syncthreads();  
  }  
  d_out[globalId] = temp[localId]; // write output 
}

__global__ void spread(const float* const d_tmp, float * const d_io, const int size) {
  int globalIdx = threadIdx.x + blockDim.x * blockIdx.x;
  if (globalIdx < size)
    d_io[globalIdx] += d_tmp[blockIdx.x];
}

__global__ void yeild_tailes(const float* const d_source, const float* const d_first_stage, float * d_out, const int size) {
  int globalIdx = threadIdx.x + blockDim.x * blockIdx.x;
  int tail_idx = blockDim.x * (blockIdx.x+1) - 1;
  if (globalIdx < size)
    d_out[blockIdx.x] = d_source[tail_idx] + d_first_stage[tail_idx];
}

// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch39.html
// TODO: In article finded bugs.
// DANGER: Work only 1 block!
// Asserts in cude kernel: http://choorucode.com/2011/03/02/cuda-assertion-in-kernel-code/
__global__ void exclusive_scan_kernel_doubled_cache(float * d_out, const float * const d_in, int n)
{ 
  // результаты работы потоков можем расшаривать через эту
  // память или через глобальную
  extern __shared__ float temp[];  
  int localId  = threadIdx.x;
  int globalId = localId;
  
  assert(blockIdx.x == 1);
  
  //  for
  int p_sink = 0;  // int1 не работает
  int p_source = 1;

  // Load input into shared memory.  
  // This is exclusive scan, so shift right by one  
  // and set first element to 0 
  float tmpVal = 0;
  if (localId > 0)
    if (globalId < n)
      tmpVal = d_in[globalId-1];
    else 
      tmpVal = 0;

  temp[p_sink * n + localId] = tmpVal;
  __syncthreads();  

  for (int offset = 1; offset < n; offset *= 2)  // 2^i
  {  
    cudaSwap(p_sink, p_source);
    
    if (localId >= offset) {
      float temp_val = temp[p_source*n + localId] + temp[p_source*n + localId - offset];
      temp[p_sink*n + localId] = temp_val;  
    } else {
      temp[p_sink*n+localId] = temp[p_source * n+localId];  
    }
    
    // буффера переписали
    __syncthreads();  
  }  

  // p_sink == 0?
  // Пишем из текущего буффера
  d_out[localId] = temp[p_sink * n + localId /*1*/]; // write output 
}

void scan_hillis_single_block(float * d_out, const float * const d_in, const int size) 
{
  
  int threads = maxThreadsPerBlock;
  int blocks = ceil((1.0f*size) / maxThreadsPerBlock);
  
  // assumes that size is not greater than maxThreadsPerBlock^2
  // and that size is a multiple of maxThreadsPerBlock
  assert(size <= threads * threads);  // для двушаговой редукции, чтобы уложиться
  assert(blocks * threads >= size);  // нужно будет ослабить - shared-mem дозаполним внутри ядер
  assert(isPow2(threads));  // должно делиться на 2 до конца - А нужно ли?
  //assert(blocks == 2);  // пока чтобы не комбинировать результаты блоков
  
  float * d_sink;
  float * d_sink_out;
  checkCudaErrors(cudaMalloc((void **) &d_sink, blocks));
  checkCudaErrors(cudaMalloc((void **) &d_sink_out, blocks));

  // Sectioned scan
  kern_exclusive_scan_cache<<< blocks, threads, threads * sizeof(float) >>>(d_in, d_out, size);
  
  // map
  yeild_tailes<<< blocks, threads >>>(d_in, d_out, d_sink, size);
  
  // запускаем scan на временном массиве
  kern_exclusive_scan_cache<<< 1, threads, threads * sizeof(float) >>>(d_sink, d_sink_out, threads);
  
  // делаем map на исходном массиве
  spread<<< blocks, threads >>>(d_sink_out, d_out, size);
  
  cudaFree(d_sink);
  cudaFree(d_sink_out);
}

#ifndef SCAN_AS_UNIT
int main(int argc, char **argv)
{
  int deviceCount;
  cudaGetDeviceCount(&deviceCount);
  if (deviceCount == 0) {
      fprintf(stderr, "error: no devices supporting CUDA.\n");
      exit(EXIT_FAILURE);
  }
  int dev = 0;
  cudaSetDevice(dev);

  cudaDeviceProp devProps;
  if (cudaGetDeviceProperties(&devProps, dev) == 0)
  {
      printf("Using device %d:\n", dev);
      printf("%s; global mem: %dB; compute v%d.%d; clock: %d kHz\n",
             devProps.name, (int)devProps.totalGlobalMem, 
             (int)devProps.major, (int)devProps.minor, 
             (int)devProps.clockRate);
  }

  const int ARRAY_SIZE = maxThreadsPerBlock * 7 - 4;
  const int ARRAY_BYTES = ARRAY_SIZE * sizeof(float);

  // Serial:
  // generate the input array on the host
  float h_in[ARRAY_SIZE];
  float h_scan_gold[ARRAY_SIZE];
  float sum = 0.0f;
  for(int i = 0; i < ARRAY_SIZE; i++) {
    h_scan_gold[i] = sum;
    h_in[i] = 1.0f * (i+1);
    sum += h_in[i];  
  }

  // Parallel
  // declare GPU memory pointers
  float * d_in, * d_out;//, * d_out;

  // allocate GPU memory
  cudaMalloc((void **) &d_in, ARRAY_BYTES);
  cudaMalloc((void **) &d_out, ARRAY_BYTES); // overallocated
  //cudaMalloc((void **) &d_out, sizeof(float));

  // transfer the input array to the GPU
  checkCudaErrors(cudaMemcpy(d_in, h_in, ARRAY_BYTES, cudaMemcpyHostToDevice)); 

  int whichKernel = 0;
  if (argc == 2) {
      whichKernel = atoi(argv[1]);
  }
      
  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  whichKernel = 0;
  switch(whichKernel) {
  case 0:
      printf("Running reduce hill exclusive\n");
      cudaEventRecord(start, 0);
      scan_hillis_single_block(d_out, d_in, ARRAY_SIZE);
      checkCudaErrors(cudaGetLastError());
      cudaEventRecord(stop, 0);
      break;
  default:
      fprintf(stderr, "error: ran no kernel\n");
      exit(EXIT_FAILURE);
  }
  cudaEventSynchronize(stop);
  float elapsedTime;
  cudaEventElapsedTime(&elapsedTime, start, stop);    
  elapsedTime /= 100.0f;      // 100 trials

  // copy back the sum from GPU
  float h_out[ARRAY_SIZE]; // ARRAY_BYTES
  checkCudaErrors(cudaMemcpy(h_out, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost));
  
  printf("average time elapsed: %f\n", elapsedTime);

  // free GPU memory allocation
  cudaFree(d_in);
  cudaFree(d_out);
  
  // Check: сравнить бы с моделью
  vector<float> hGold;
  vector<float> hOut;
  unsigned dataArraySize = sizeof(h_scan_gold) / sizeof(float);
  assert(dataArraySize == ARRAY_SIZE);
  hGold.insert(hGold.end(), &h_scan_gold[0], &h_scan_gold[dataArraySize]);
  hOut.insert(hOut.end(), &h_out[0], &h_out[dataArraySize]);
  assert(hOut.size() == hGold.size());
  assert(
  equal
  //for_each
    //equal_adapt
    (hGold.begin(), hGold.end(), hOut.begin(), AlmostEqualPredicate)//;
  );
  
     
  return 0;
}
#endif
