// TODO: сделать min and max reduce not in place

// C
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>

// C++
#include <vector>
#include <algorithm>

// 3rdparty
#include <cuda_runtime.h>
const int maxThreadsPerBlock = 1024;

// http://habrahabr.ru/post/146793/ !! трюки на С++

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
  const float epsilon = 1e-2;/* some small number such as 1e-5 */;
  //printf("Delta = %f\n", x -y);
  //printf("x = %f\n", x);
  //printf("y = %f\n", y);
  return std::abs(x - y) <= epsilon * std::abs(x);
  // see Knuth section 4.2.2 pages 217-218
}

inline int isPow2(int a) {
  return !(a&(a-1));
}

// http://valera.asf.ru/cpp/book/c10.html
//#define max_cuda( a, b ) ( ((a) > (b)) ? (a) : (b) )
//#define min_cuda( a, b ) ( ((a) < (b)) ? (a) : (b) )

// Нейтральные элементы
// http://stackoverflow.com/questions/2684603/how-do-i-initialize-a-float-to-its-max-min-value

template <class Type> __device__ Type min_cuda( Type a, Type b ) {
  // I - +inf
  return a < b ? a : b;
}

template <class Type> __device__ Type max_cuda( Type a, Type b ) {
  // I - -inf
  return a > b ? a : b;
}

using std::vector;

__global__ void shmem_max_reduce_kernel(
    float * d_out, 
    const float * d_in /*для задания важна константность*/,
    const int size)
{
    // sdata is allocated in the kernel call: 3rd arg to <<<b, t, shmem>>>
    extern __shared__ float sdata[];

    int myId = threadIdx.x + blockDim.x * blockIdx.x;
    int tid  = threadIdx.x;

    // load shared mem from global mem
    if (myId < size)
      sdata[tid] = d_in[myId];
    else {
      // заполняем нейтральными элементами
      sdata[tid] = -FLT_MAX;
    }
    
    __syncthreads();            // make sure entire block is loaded!
    
    //assert(isPow2(blockDim.x));  // нельзя

    // do reduction in shared mem
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s)
        {
          float tmp =  max_cuda<float>(sdata[tid], sdata[tid + s]); 
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

__global__ void shmem_min_reduce_kernel(
    float * d_out, 
    const float * d_in /*для задания важна константность*/, int size)
{
    // sdata is allocated in the kernel call: 3rd arg to <<<b, t, shmem>>>
    extern __shared__ float sdata[];

    int myId = threadIdx.x + blockDim.x * blockIdx.x;
    int tid  = threadIdx.x;

    // load shared mem from global mem
    if (myId < size)
      sdata[tid] = d_in[myId];
    else {
      // заполняем нейтральными элементами
      sdata[tid] = +FLT_MAX;
    }
    
    __syncthreads();            // make sure entire block is loaded!

    // do reduction in shared mem
    //TODO: blockDim должна быть степенью 2
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s)
        {
          float tmp =  min_cuda<float>(sdata[tid], sdata[tid + s]); 
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

//TODO: не хотелось писать в сигнатуру, хотя удобство сомнительно
template<bool isMin>  
void reduce_shared_min(
    float * const d_out, 
    float * const d_intermediate, float const * const d_in, 
    int size
    //, bool isMin
    ) 
{
  int threads = maxThreadsPerBlock;
  int blocks = size / threads;  // отбрасываем дробную часть
  
  // assumes that size is not greater than maxThreadsPerBlock^2
  // and that size is a multiple of maxThreadsPerBlock
  assert(size <= threads * threads);  // для двушаговой редукции, чтобы уложиться
  //assert(blocks * threads == size);  // нужно будет ослабить - shared-mem дозаполним внутри ядер
  assert(isPow2(threads));  // должно делиться на 2 до конца

  // Step 1: Вычисляем результаты для каждого блока
  if (isMin)
    shmem_min_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_intermediate, d_in, size);
  else {
    shmem_max_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_intermediate, d_in, size);
  }

  // Step 2: Комбинируем разультаты блоков и это ограничение на размер входных данных
  // now we're down to one block left, so reduce it
  threads = blocks; // launch one thread for each block in prev step
  blocks = 1;
  if (isMin)
    shmem_min_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_out, d_intermediate, threads);
  else {
    shmem_max_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_out, d_intermediate, threads);
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

  const int ARRAY_SIZE = (1 << 19);  //TODO: важно правильно выбрать
  const int ARRAY_BYTES = ARRAY_SIZE * sizeof(float);

  // generate the input array on the host
  float h_in[ARRAY_SIZE];
  for(int i = 0; i < ARRAY_SIZE; i++) {
      // generate random float in [-1.0f, 1.0f]
      h_in[i] = -1.0f + (float)random()/((float)RAND_MAX/2.0f);
  }
  h_in[ARRAY_SIZE-1] = -1000.0;
  h_in[0] = 1000.0;
  
  // Ищем минимум
  // http://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping
  vector<float> hIn;
  unsigned dataArraySize = sizeof(h_in) / sizeof(float);
  assert(dataArraySize == ARRAY_SIZE);
  hIn.insert(hIn.end(), &h_in[0], &h_in[dataArraySize]);
  assert(hIn.size() == ARRAY_SIZE);
  
  // Используем стандартную функцию
  // http://stackoverflow.com/questions/8340569/stdvector-and-stdmin-behavior 
  // Похоже можно искать сразу в векторе
  float serialMin = *std::min_element(hIn.begin(),hIn.end());
  float serialMax = *std::max_element(hIn.begin(),hIn.end());


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
    
  {     
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    // launch the kernel
    switch(whichKernel) {
    case 0:
	printf("Running min reduce with shared mem\n");
	cudaEventRecord(start, 0);
	for (int i = 0; i < 100; i++)
	{
	    reduce_shared_min<true>(d_out, d_intermediate, d_in, ARRAY_SIZE);//, true);
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
    
    assert(isEqual(h_out, serialMin));
    printf("average time elapsed: %f\n", elapsedTime);
  }
  
  // MAX
  {     
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    // launch the kernel
    switch(whichKernel) {
    case 0:
	printf("Running min reduce with shared mem\n");
	cudaEventRecord(start, 0);
	//for (int i = 0; i < 100; i++)
	//{
	    reduce_shared_min<false>(d_out, d_intermediate, d_in, ARRAY_SIZE);//, false);
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
    
    assert(isEqual(h_out, serialMax));
    printf("average time elapsed: %f\n", elapsedTime);
  }

  // free GPU memory allocation
  cudaFree(d_in);
  cudaFree(d_intermediate);
  cudaFree(d_out);
  return 0;
}
