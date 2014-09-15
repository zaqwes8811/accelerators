#include "splitters.h"

// C
#include <math.h>

// Third party
#include <gtest/gtest.h>

TEST(Split, OptimalSplit) {
  const int kColumns = 311;
  const int kRows = 234;
  const int kCellSize = 512;

  const float kRowToColumns = (1.0f * kRows) / kColumns;
  printf("%0.2f\n", kRowToColumns);

  float yRaw = sqrt(1.0f * kCellSize / kRowToColumns);
  float xRaw = kRowToColumns * yRaw;
  printf("%0.2f %0.2f\n", xRaw, yRaw);

  int y = (int)floor(yRaw);
  int x = (int)floor(xRaw);
  printf("%d %d space = %d\n", x, y, x*y);

  EXPECT_GE(kCellSize, x*y);
}

TEST(Split, OptimalSplitRelease) {
  // ACuda_ij = AMatrix_ij^T;
  // cuda_x -> j - колонка
  // cuda_y -> i - ряд
  const size_t kColumns = 311;
  const size_t kRows = 234;
  const size_t kCellSize = 512;
  const layout2d_t layout = spliGetOpt2DParams(kRows, kColumns, kCellSize);
  size_t space = layout.grid.x * layout.block.x * layout.grid.y * layout.block.y;

  EXPECT_GE(kCellSize, layout.block.x * layout.block.y);
  EXPECT_LE(kRows, layout.grid.x * layout.block.x);
  EXPECT_LE(kColumns, layout.grid.y * layout.block.y);
  EXPECT_LE(kRows * kColumns, space);
  printf("GX = %d GY = %d\n", layout.grid.x, layout.grid.y); 
  printf("BX = %d BY = %d\n", layout.block.x, layout.block.y); 
  printf("space_img = %d space_calc = %d\n", kRows * kColumns, space);
  printf("cell = %d cell_max = %d\n", layout.block.x * layout.block.y, kCellSize); 
}

TEST(Split, OptimalPreciseFit) {
  // ACuda_ij = AMatrix_ij^T;
  // cuda_x -> j - колонка
  // cuda_y -> i - ряд
  const size_t kColumns = 7;
  const size_t kRows = 512;
  const size_t kCellSize = 1024;
  const layout2d_t layout = spliGetOpt2DParams(kRows, kColumns, kCellSize);
  size_t space = layout.grid.x * layout.block.x * layout.grid.y * layout.block.y;

  EXPECT_GE(kCellSize, layout.block.x * layout.block.y);
  EXPECT_LE(kRows, layout.grid.x * layout.block.x);
  EXPECT_LE(kColumns, layout.grid.y * layout.block.y);
  EXPECT_LE(kRows * kColumns, space);
  printf("GX = %d GY = %d\n", layout.grid.x, layout.grid.y); 
  printf("BX = %d BY = %d\n", layout.block.x, layout.block.y); 
  printf("space_img = %d space_calc = %d\n", kRows * kColumns, space);
  printf("cell = %d cell_max = %d\n", layout.block.x * layout.block.y, kCellSize); 
}
