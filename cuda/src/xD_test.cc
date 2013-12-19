// Third party
#include <gtest/gtest.h>
#include "cs344/summary/reference_calc.h"

#include "cs344/HW2/hw2_kernels_cu.h"
#include "cs344/reuse/utils.h"

typedef unsigned char uint8_t;

TEST(xD, Base) {
  const size_t ROWS = 7;
  const size_t COLUMNS = 9;
  const size_t FILTER_WIDTH = 3;
  // Filter
  float const filter2D[FILTER_WIDTH][FILTER_WIDTH] = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
  };
  float h_filter1D[sizeof filter2D];

  for (int r = 0; r < FILTER_WIDTH; r++) {
    for (int c = 0; c < FILTER_WIDTH; c++) {
      h_filter1D[r * COLUMNS + c] = filter2D[r][c];
    }
  }

  // Image
  uint8_t h_image2D[ROWS][COLUMNS];
  uint8_t h_OutImage2D[ROWS][COLUMNS];
  uint8_t h_InImage1D[sizeof h_image2D];

  uint8_t h_Outimage1D[sizeof h_image2D];

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      h_image2D[r][c] = c;
      h_InImage1D[r * COLUMNS + c] = c;
    }
  }

  EXPECT_EQ(h_image2D[4][3], h_InImage1D[4 * COLUMNS + 3]);

  /// /// ///

  // —читаем через ref-функцию
  channelConvolutionRefa(
    h_InImage1D,
    h_Outimage1D,
    ROWS, COLUMNS,
    h_filter1D, FILTER_WIDTH);

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


  run_test_blur(
      d_InImage1D, d_OutImage1D, ROWS, COLUMNS,
      d_filter1D, FILTER_WIDTH);

  checkCudaErrors(
    cudaMemcpy(h_Outimage1D, d_OutImage1D, sizeof(unsigned char) * numPixels, cudaMemcpyDeviceToHost));

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      h_OutImage2D[r][c] = h_Outimage1D[r * COLUMNS + c];
    }
  }


  checkCudaErrors(cudaFree(d_filter1D));
  checkCudaErrors(cudaFree(d_OutImage1D));
  checkCudaErrors(cudaFree(d_InImage1D));
}