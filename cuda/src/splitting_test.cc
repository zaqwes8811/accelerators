
// Third party
#include <gtest/gtest.h>
#include <vector_types.h>

typedef struct layout2d_s {
  const dim3 block;
  const dim3 grid;
} layout2d_t;

layout2d_t cureGetOpt2DParams(
    const size_t kRows, 
    const size_t kColumns, 
    const size_t kCellSize) 
  {
  const float kRowToColumns = (1.0f * kRows) / kColumns;
  float yRaw = sqrt(1.0f * kCellSize / kRowToColumns);
  float xRaw = kRowToColumns * yRaw;
  int y = (int)floor(yRaw);
  int x = (int)floor(xRaw);

  dim3 blockSize;
  blockSize.x = x;
  blockSize.y = y;
  blockSize.z = 1;

  // »щем размерность сетки
  float xGRowsRaw = (1.0f * kRows) / x;
  float yGRowsRaw = (1.0f * kColumns) / y;

  dim3 gridSize;
  gridSize.x = (int)ceil(xGRowsRaw);
  gridSize.y = (int)ceil(yGRowsRaw);
  gridSize.z = 1;
  const layout2d_t layout = {blockSize, gridSize};
  printf("%0.2f %0.2f\n", xRaw, yRaw);
  return layout;
}

TEST(Split, OptimalSplit) {
  const int kColumns = 311;
  const int kRows = 234;
  const int kCellSize = 1024;

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
  // cuda_y -> i - р€д
  const size_t kColumns = 311;
  const size_t kRows = 234;
  const size_t kCellSize = 1024;
  const layout2d_t layout = cureGetOpt2DParams(kRows, kColumns, kCellSize);
  size_t space = layout.grid.x * layout.block.x * layout.grid.y * layout.block.y;

  EXPECT_GE(kCellSize, layout.block.x * layout.block.y);
  EXPECT_LE(kRows, layout.grid.x * layout.block.x);
  EXPECT_LE(kColumns, layout.grid.y * layout.block.y);
  EXPECT_LE(kRows * kColumns, space);
  printf("GX = %d GY = %d\n", layout.grid.x, layout.grid.y); 
  printf("BX = %d BY = %d\n", layout.block.x, layout.block.y); 
  printf("space_img = %d space_calc = %d\n", kRows * kColumns, space);
}

TEST(Split, OptimalPreciseFit) {
  // ACuda_ij = AMatrix_ij^T;
  // cuda_x -> j - колонка
  // cuda_y -> i - р€д
  const size_t kColumns = 1024/2;
  const size_t kRows = 1024;
  const size_t kCellSize = 1024;
  const layout2d_t layout = cureGetOpt2DParams(kRows, kColumns, kCellSize);
  size_t space = layout.grid.x * layout.block.x * layout.grid.y * layout.block.y;

  EXPECT_GE(kCellSize, layout.block.x * layout.block.y);
  EXPECT_EQ(kRows, layout.grid.x * layout.block.x);
  EXPECT_EQ(kColumns, layout.grid.y * layout.block.y);
  EXPECT_EQ(kRows * kColumns, space);
  printf("GX = %d GY = %d\n", layout.grid.x, layout.grid.y); 
  printf("BX = %d BY = %d\n", layout.block.x, layout.block.y); 
  printf("space_img = %d space_calc = %d\n", kRows * kColumns, space);
  printf("cell = %d cell_max = %d\n", layout.block.x * layout.block.y, kCellSize); 
}
