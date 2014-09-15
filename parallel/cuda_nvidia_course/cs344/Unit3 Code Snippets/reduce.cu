// TODO: сделать min and max reduce not in place

// C
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 3rdparty
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

// Float comparison http://floating-point-gui.de/errors/comparison/ - Java sample
// http://www.parashift.com/c++-faq/floating-point-arith.html
// http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html - матчасть
#include <cmath>  /* for std::abs(double) */

// не коммутативное
// isEqual(x,y) != isEqual(y,x)
inline bool isEqual(float x, float y)
{
  const float epsilon = 1e-5;/* some small number such as 1e-5 */;
  printf("Delta = %f\n", x -y);
  return std::abs(x - y) <= epsilon * std::abs(x);
  // see Knuth section 4.2.2 pages 217-218
}


// Работает in-place
__global__ void global_reduce_kernel(float * d_out, float * d_in)
{
    int myId = threadIdx.x + blockDim.x * blockIdx.x;  // not one block!
    int tid  = threadIdx.x;

    // do reduction in global mem
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s)
        {
            d_in[myId] += d_in[myId + s];  // модиф. вх. массив!
        }
        __syncthreads();        // make sure all adds at one stage are done!
    }

    // only thread 0 writes result for this block back to global mem
    if (tid == 0)
    {
        d_out[blockIdx.x] = d_in[myId];
    }
}

__global__ void shmem_reduce_kernel(
    float * d_out, 
    const float * d_in /*для задания важна константность*/)
{
    // sdata is allocated in the kernel call: 3rd arg to <<<b, t, shmem>>>
    extern __shared__ float sdata[];

    int myId = threadIdx.x + blockDim.x * blockIdx.x;
    int tid  = threadIdx.x;

    // load shared mem from global mem
    sdata[tid] = d_in[myId];
    __syncthreads();            // make sure entire block is loaded!

    // do reduction in shared mem
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s)
        {
          float tmp =  sdata[tid] + sdata[tid + s]; 
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

void reduce_shared_min(
    float * d_out, 
    float * d_intermediate, 
    float * d_in, 
    int size) 
  {
  // assumes that size is not greater than maxThreadsPerBlock^2
  // and that size is a multiple of maxThreadsPerBlock
  const int maxThreadsPerBlock = 1024;
  int threads = maxThreadsPerBlock;
  int blocks = size / maxThreadsPerBlock;

  // Step 1:
  shmem_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_intermediate, d_in);

  // Step 2:
  // TODO: Комбинируем разультаты блоков и это ограничение на размер входных данных
  // now we're down to one block left, so reduce it
  threads = blocks; // launch one thread for each block in prev step
  blocks = 1;
  shmem_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_out, d_intermediate);
}

void reduce(float * d_out, float * d_intermediate, float * d_in, 
            int size, bool usesSharedMemory)
{
    // assumes that size is not greater than maxThreadsPerBlock^2
    // and that size is a multiple of maxThreadsPerBlock
    const int maxThreadsPerBlock = 1024;
    int threads = maxThreadsPerBlock;
    int blocks = size / maxThreadsPerBlock;

    // Step 1:
    if (usesSharedMemory)
    {
        shmem_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>
            (d_intermediate, d_in);
    }
    else
    {
        global_reduce_kernel<<<blocks, threads>>>
            (d_intermediate, d_in);
    }

    // Step 2:
    // TODO: Комбинируем разультаты блоков и это ограничение на размер входных данных
    // now we're down to one block left, so reduce it
    threads = blocks; // launch one thread for each block in prev step
    blocks = 1;
    if (usesSharedMemory)
    {
        shmem_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>
            (d_out, d_intermediate);
    }
    else
    {
        global_reduce_kernel<<<blocks, threads>>>
            (d_out, d_intermediate);
    }
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

    const int ARRAY_SIZE = 1 << 20;
    const int ARRAY_BYTES = ARRAY_SIZE * sizeof(float);

    // generate the input array on the host
    float h_in[ARRAY_SIZE];
    float sum = 0.0f;
    for(int i = 0; i < ARRAY_SIZE; i++) {
        // generate random float in [-1.0f, 1.0f]
        h_in[i] = -1.0f + (float)random()/((float)RAND_MAX/2.0f);
        sum += h_in[i];
    }
    
    // Ищем минимум

    // declare GPU memory pointers
    float * d_in;
    float * d_intermediate;  // stage 1 result
    float * d_out;

    // allocate GPU memory
    cudaMalloc((void **) &d_in, ARRAY_BYTES);
    cudaMalloc((void **) &d_intermediate, ARRAY_BYTES); // overallocated
    cudaMalloc((void **) &d_out, sizeof(float));  // 1 значение

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
    switch(whichKernel) {
    case 0:
        printf("Running global reduce\n");
        cudaEventRecord(start, 0);
        //for (int i = 0; i < 100; i++)
        //{
            reduce(d_out, d_intermediate, d_in, ARRAY_SIZE, false);
        //}
        cudaEventRecord(stop, 0);
        break;
    case 1:
        printf("Running reduce with shared mem\n");
        cudaEventRecord(start, 0);
        //for (int i = 0; i < 100; i++)
        //{
            reduce(d_out, d_intermediate, d_in, ARRAY_SIZE, true);
        //}
        cudaEventRecord(stop, 0);
        break;
  case 2:
        printf("Running min reduce with shared mem\n");
        cudaEventRecord(start, 0);
        //for (int i = 0; i < 100; i++)
        //{
            reduce_shared_min(d_out, d_intermediate, d_in, ARRAY_SIZE);
        //}
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
    
    assert(isEqual(h_out, sum));

    printf("average time elapsed: %f\n", elapsedTime);

    // free GPU memory allocation
    cudaFree(d_in);
    cudaFree(d_intermediate);
    cudaFree(d_out);
    return 0;
}
