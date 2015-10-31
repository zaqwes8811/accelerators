#include "float_ops.h"

#include <cuda_runtime.h>

#include <iostream>
#include <vector>
#include <algorithm> 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

using std::vector;
using std::equal;
using std::for_each;

#define checkCudaErrors(val) check( (val), #val, __FILE__, __LINE__)

extern void scan_hillis_single_block(
  float * d_out, const float * const d_in, const int size);

template<typename T>
void check(T err, const char* const func, const char* const file, const int line) {
  if (err != cudaSuccess) {
    std::cerr << "CUDA error at: " << file << ":" << line << std::endl;
    std::cerr << cudaGetErrorString(err) << " " << func << std::endl;
    exit(EXIT_FAILURE);
  }
}

float rand_logic_value() 
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
  const int KBytesInArray = kArraySize * sizeof(float);

  // Serial:
  // generate the input array on the host
  float h_in[kArraySize];
  vector<float> h_gold;
  vector<float> h_out(kArraySize, 0);
  float sum = 0;
  for(int i = 0; i < kArraySize; i++) {
    h_gold.push_back(sum);
    float tmp = i+1;
    h_in[i] = tmp;
    sum += tmp;
  }
  
  // Parallel
  // declare GPU memory pointers
  float * d_in, * d_out, * d_predicat;
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
    checkCudaErrors(cudaMemcpy(&h_out[0], d_out, KBytesInArray, cudaMemcpyDeviceToHost));
    
    printf("average time elapsed: %f\n", elapsedTime);

    // free GPU memory allocation
    cudaFree(d_in);
    cudaFree(d_out);
  }
  
  /// Check result
  assert(h_out.size() == h_gold.size());
  // раз значения uint можно просто проверить оператором ==
  assert(equal(h_gold.begin(), h_gold.end(), h_out.begin()
  //, AlmostEqualPredicate
  ));
  return 0;
}

