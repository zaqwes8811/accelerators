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

extern void scan_hillis_single_block(const unsigned int * const d_in, unsigned int * const d_out, const int size);

template<typename T>
void check(T err, const char* const func, const char* const file, const int line) {
  if (err != cudaSuccess) {
    std::cerr << "CUDA error at: " << file << ":" << line << std::endl;
    std::cerr << cudaGetErrorString(err) << " " << func << std::endl;
    //assert(false && "CUDA error");
    exit(1);
  }
}

using std::vector;
using std::equal;
using std::for_each;

int main(int argc, char **argv)
{
  /// Check device
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
  
  int whichKernel = 0;
  if (argc == 2) {
      whichKernel = atoi(argv[1]);
  }

  /// Real work
  const int maxThreadsPerBlock = 8;
  const int ARRAY_SIZE = maxThreadsPerBlock * 2 - 1;
  const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned int);

  // Serial:
  // generate the input array on the host
  unsigned int h_in[ARRAY_SIZE];
  unsigned int h_scan_gold[ARRAY_SIZE];
  unsigned int sum = 0;
  for(int i = 0; i < ARRAY_SIZE; i++) {
    h_scan_gold[i] = sum;
    h_in[i] = i+1;
    sum += h_in[i];  
  }

  // Parallel
  // declare GPU memory pointers
  unsigned int * d_in, * d_out;//, * d_out;

  // allocate GPU memory
  cudaMalloc((void **) &d_in, ARRAY_BYTES);
  cudaMalloc((void **) &d_out, ARRAY_BYTES); // overallocated

  // transfer the input array to the GPU
  checkCudaErrors(cudaMemcpy(d_in, h_in, ARRAY_BYTES, cudaMemcpyHostToDevice)); 
  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  switch(whichKernel) {
  case 0:
      printf("Running reduce hill exclusive\n");
      cudaEventRecord(start, 0);
      scan_hillis_single_block(d_in, d_out, ARRAY_SIZE);
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
  unsigned int h_out[ARRAY_SIZE]; // ARRAY_BYTES
  checkCudaErrors(cudaMemcpy(h_out, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost));
  
  printf("average time elapsed: %f\n", elapsedTime);

  // free GPU memory allocation
  cudaFree(d_in);
  cudaFree(d_out);
  
  /// Check result
  vector<unsigned int> hGold;
  vector<unsigned int> hOut;
  unsigned dataArraySize = sizeof(h_scan_gold) / sizeof(unsigned int);
  assert(dataArraySize == ARRAY_SIZE);
  hGold.insert(hGold.end(), &h_scan_gold[0], &h_scan_gold[dataArraySize]);
  hOut.insert(hOut.end(), &h_out[0], &h_out[dataArraySize]);
  assert(hOut.size() == hGold.size());
  assert(equal(hGold.begin(), hGold.end(), hOut.begin()
  //, AlmostEqualPredicate
  ));
  return 0;
}

