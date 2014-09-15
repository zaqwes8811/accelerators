// Third party
#include <gtest/gtest.h>
#include "cs344/summary/reference_calc.h"

#include "cuda/hw2_kernels_cu.h"
#include "cs344/reuse/utils.h"

typedef unsigned char uint8_t;

TEST(xD, Base2) {
  const size_t ROWS = 7;
  const size_t COLUMNS = 9;
  const size_t FILTER_WIDTH = 3;
  // Filter
  float const FILTER_2D[FILTER_WIDTH][FILTER_WIDTH] = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
  };
  float h_filter1D[FILTER_WIDTH * FILTER_WIDTH];

  for (int x = 0; x < FILTER_WIDTH; x++) {
    for (int y = 0; y < FILTER_WIDTH; y++) {
      h_filter1D[x * FILTER_WIDTH + y] = FILTER_2D[x][y];
    }
  }

  // Image
  uint8_t h_InImage2D[ROWS][COLUMNS];
  uint8_t h_OutImage2D[ROWS][COLUMNS];
  uint8_t h_InImage1D[sizeof h_InImage2D];

  uint8_t h_OutImage1D[sizeof h_InImage2D];

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      h_InImage2D[r][c] = c;
      h_InImage1D[r * COLUMNS + c] = 1;
    }
  }

  //EXPECT_EQ(h_InImage2D[4][3], h_InImage1D[4 * COLUMNS + 3]);

  /// /// ///

  // ������� ����� ref-�������
  channelConvolutionRefa(
    h_InImage1D,
    h_OutImage1D,
    ROWS, COLUMNS,
    h_filter1D, FILTER_WIDTH);

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      unsigned int value = h_OutImage1D[r * COLUMNS + c];
      printf("%u ", value);
    }
    printf("\n");
  }

  // CUDA
  uint8_t* d_InImage1D;
  uint8_t* d_OutImage1D;
  float* d_filter1D;
  
  // Allocate on GPU side
  const int numPixels = ROWS * COLUMNS;
  checkCudaErrors(cudaMalloc((void**)&d_InImage1D, sizeof(unsigned char) * numPixels));
  checkCudaErrors(
    cudaMemcpy(d_InImage1D, h_InImage1D, sizeof(unsigned char) * numPixels, cudaMemcpyHostToDevice));

  checkCudaErrors(cudaMalloc((void**)&d_OutImage1D, sizeof(unsigned char) * numPixels));
  checkCudaErrors(cudaMemset(d_OutImage1D, 0, numPixels * sizeof(unsigned char)));

  // Put filter
  checkCudaErrors(cudaMalloc((void**)&d_filter1D, sizeof(unsigned char) * FILTER_WIDTH));
  checkCudaErrors(
    cudaMemcpy(d_filter1D, h_filter1D, sizeof(unsigned char) * FILTER_WIDTH, cudaMemcpyHostToDevice));


  cuinRunOnlyBlurTest(
      d_InImage1D, d_OutImage1D, ROWS, COLUMNS,
      d_filter1D, FILTER_WIDTH);

  checkCudaErrors(
    cudaMemcpy(h_OutImage1D, d_OutImage1D, sizeof(unsigned char) * numPixels, cudaMemcpyDeviceToHost));

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      h_OutImage2D[r][c] = h_OutImage1D[r * COLUMNS + c];
      printf("%d ", h_OutImage2D[r][c]);
    }
    printf("\n");
  }


  checkCudaErrors(cudaFree(d_filter1D));
  checkCudaErrors(cudaFree(d_OutImage1D));
  checkCudaErrors(cudaFree(d_InImage1D));
}
