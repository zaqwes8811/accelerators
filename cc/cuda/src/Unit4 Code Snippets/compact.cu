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
    exit(EXIT_FAILURE);
  }
}

using std::vector;
using std::equal;
using std::for_each;

unsigned int rand_logic_value() 
{
  return rand() % 2;
}

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
  const int kArraySize = maxThreadsPerBlock * 2 - 1;
  const int KBytesInArray = kArraySize * sizeof(unsigned int);

  // Serial:
  // generate the input array on the host
  unsigned int h_in[kArraySize];
  vector<unsigned int> hGold;
  vector<unsigned int> hOut(kArraySize, 0);
  unsigned int sum = 0;
  for(int i = 0; i < kArraySize; i++) {
    hGold.push_back(sum);
    unsigned int tmp = i+1;
    h_in[i] = tmp;
    sum += tmp;
  }
  
  // Parallel
  // declare GPU memory pointers
  unsigned int * d_in, * d_out, * d_predicat;
  {
    // allocate GPU memory
    cudaMalloc((void **) &d_in, KBytesInArray);
    cudaMalloc((void **) &d_out, KBytesInArray); // overallocated

    // transfer the input array to the GPU
    checkCudaErrors(cudaMemcpy(d_in, h_in, KBytesInArray, cudaMemcpyHostToDevice)); 
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    switch(whichKernel) {
    case 0:
	printf("Running reduce hill exclusive\n");
	cudaEventRecord(start, 0);
	scan_hillis_single_block(d_in, d_out, kArraySize);
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
    checkCudaErrors(cudaMemcpy(&hOut[0], d_out, KBytesInArray, cudaMemcpyDeviceToHost));
    
    printf("average time elapsed: %f\n", elapsedTime);

    // free GPU memory allocation
    cudaFree(d_in);
    cudaFree(d_out);
  }
  
  /// Check result
  assert(hOut.size() == hGold.size());
  // раз значения uint можно просто проверить оператором ==
  assert(equal(hGold.begin(), hGold.end(), hOut.begin()
  //, AlmostEqualPredicate
  ));
  return 0;
}

