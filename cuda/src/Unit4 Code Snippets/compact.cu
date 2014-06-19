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
      //scan_hillis_single_block(d_out, d_in, ARRAY_SIZE);
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

