// Own
#include "hw2_kernels_cu.h"

// C
#include <stdio.h>

// Third party
#include "cs344/reuse/utils.h"

void cuinRunOnlyBlurTest(
    const unsigned char* const inputChannel,
    unsigned char* const outputChannel,
    int numRows, int numCols,
    const float* const filter, const int filterWidth) 
  {
  //You must fill in the correct sizes for the blockSize and gridSize
  //currently only one block with one thread is being launched
  const dim3 gridSize(1, 1, 1);  //TODO

  const dim3 blockSize(numRows*2, numCols, 1);  //TODO
  
  printf("%s = %i\n", "numCols", numCols);
  printf("%s = %i\n", "numRows", numRows);
  /*gaussian_blur<<<gridSize, blockSize>>>(
      inputChannel, outputChannel, 
      numRows, numCols, 
      filter, filterWidth);*/
  
  cudaDeviceSynchronize(); 
  checkCudaErrors(cudaGetLastError());
}
