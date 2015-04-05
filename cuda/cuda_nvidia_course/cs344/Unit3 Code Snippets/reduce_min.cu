// Copy: Udacity "Intro in parallel computing"
//
//
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

// App
#include "float_ops.h"

using std::vector;

// http://codepad.org/TEgVmOo0

/*
 typedef float (*p_op)(const float& x, const float& y);

__device__ float min_op(const float& x, const float& y)
{
    return min(x, y);
}

__device__ float max_op(const float& x, const float& y)
{
    return max(x, y);
}

template <p_op op>
__global__
void kernel(args)
{
    ...
    data_2 = op(data_0, data_1);
    ...
}

kernel<min_op><<<config>>>(args);
 */

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

// http://valera.asf.ru/cpp/book/c10.html
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

/*
class IElem {
public:
  explicit IElem(float value) : value_(value) {}
  float get() const { return value_; }
  
private:
  const float value_;
};*/

// /*, IElem elem - doesn't work*/ /*, float I_elem - not supported*/ - такие не типовые параметрые не компилируются
//template<float (* const operation)(float, float), float (* const neutral)()>

//TODO: Injection operation - failed.
// Don't work in homework. Strange but it is it! При разыменовании функтора все портится
// Тут работает, там нет. Отличие в том, что указатель передается через несколько вызовов, хотя может это ничего не значит.
// DANGER: does't work but here work.
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
  explicit ComparatorMax(float value) : I_val(value) {}
  
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
    const int size, const ReduceOperation* const op)
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
      sdata[tid] = op->I();//-FLT_MAX;
    }
    
    __syncthreads();            // make sure entire block is loaded!
    
    //assert(isPow2(blockDim.x));  // нельзя
    //ComparatorMax op;  // нужно передать извне

    // do reduction in shared mem
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s)
        {
          float tmp =  
          max_cuda<float>
          //op
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
void reduce_shared_min(float * const d_out, float const * const d_in, int size) 
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
  shmem_min_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_intermediate, d_in, size);

  // Step 2: Комбинируем разультаты блоков и это ограничение на размер входных данных
  // now we're down to one block left, so reduce it
  threads = blocks; // launch one thread for each block in prev step
  blocks = 1;
  shmem_min_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_out, d_intermediate, threads);
  cudaFree(d_intermediate);
}

void reduce_shared_max(float * const d_out, float const * const d_in, int size) 
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
  
  ComparatorMax op;

  // Step 1: Вычисляем результаты для каждого блока
  shmem_max_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_intermediate, d_in, size, &op);

  // Step 2: Комбинируем разультаты блоков и это ограничение на размер входных данных
  // now we're down to one block left, so reduce it
  threads = blocks; // launch one thread for each block in prev step
  blocks = 1;
  shmem_max_reduce_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_out, d_intermediate, threads, &op);
  
  cudaFree(d_intermediate);
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

  const int ARRAY_SIZE = (1 << 19) - 5;  //TODO: важно правильно выбрать
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
  float * d_out;

  // allocate GPU memory
  cudaMalloc((void **) &d_in, ARRAY_BYTES);
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
	    reduce_shared_min(d_out, d_in, ARRAY_SIZE);//, true);
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
	    reduce_shared_max(d_out, d_in, ARRAY_SIZE);//, false);
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
  cudaFree(d_out);
  return 0;
}
