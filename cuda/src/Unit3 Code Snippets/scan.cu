 #include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

// Scan: 
// 1. Serial reguces - проблема в том, что если использовать reduce из лекции, то он портит исходный массив.
//   а значить нужны локальные копии для каждого потока. Work in place.
//   http://stackoverflow.com/questions/2187189/creating-arrays-in-nvidia-cuda-kernel - может потребоватся огромная память.
//
// 2.
//
// 3.
//
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch39.html

const int maxThreadsPerBlock = 1024;

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
//   хотя нет, похоже дело в том что расчет in-place
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

__global__ void global_scan_kernel_one_block(float * d_out, float * d_in)
{
//    int myId = threadIdx.x + blockDim.x * blockIdx.x;  // not one block!
    int tid  = threadIdx.x;
}

void scan(float * d_out, float * d_intermediate, float * d_in, int size) 
{
  // Precond:
  // assumes that size is not greater than maxThreadsPerBlock^2
  // and that size is a multiple of maxThreadsPerBlock

  int threads = maxThreadsPerBlock;
  int blocks = 1;//size / maxThreadsPerBlock;
  printf("Count blocks: %d\n", blocks);
  global_scan_kernel_one_block<<<blocks, threads>>>(d_intermediate, d_in);
}

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

    const int ARRAY_SIZE = maxThreadsPerBlock;//1 << 20;
    const int ARRAY_BYTES = ARRAY_SIZE * sizeof(float);

    // generate the input array on the host
    float h_in[ARRAY_SIZE];
    float sum = 0.0f;
    for(int i = 0; i < ARRAY_SIZE; i++) {
        // generate random float in [-1.0f, 1.0f]
        h_in[i] = -1.0f + (float)random()/((float)RAND_MAX/2.0f);
        sum += h_in[i];
    }

    // declare GPU memory pointers
    float * d_in, * d_intermediate, * d_out;

    // allocate GPU memory
    cudaMalloc((void **) &d_in, ARRAY_BYTES);
    cudaMalloc((void **) &d_intermediate, ARRAY_BYTES); // overallocated
    cudaMalloc((void **) &d_out, sizeof(float));

    // transfer the input array to the GPU
    cudaMemcpy(d_in, h_in, ARRAY_BYTES, cudaMemcpyHostToDevice); 

    int whichKernel = 0;
    if (argc == 2) {
        whichKernel = atoi(argv[1]);
    }
        
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    // launch the kernel
  int countTries = 1;
    switch(whichKernel) {
    case 0:
        printf("Running global reduce\n");
        cudaEventRecord(start, 0);
        for (int i = 0; i < countTries; i++)
        {
            scan(d_out, d_intermediate, d_in, ARRAY_SIZE);//, false);
        }
        cudaEventRecord(stop, 0);
        break;
    case 1:
        printf("Running reduce with shared mem\n");
        cudaEventRecord(start, 0);
        for (int i = 0; i < countTries; i++)
        {
            scan(d_out, d_intermediate, d_in, ARRAY_SIZE);//, true);
        }
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
    float h_out;
    cudaMemcpy(&h_out, d_out, sizeof(float), cudaMemcpyDeviceToHost);

    printf("average time elapsed: %f\n", elapsedTime);

    // free GPU memory allocation
    cudaFree(d_in);
    cudaFree(d_intermediate);
    cudaFree(d_out);
        
    return 0;
}
