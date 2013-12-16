

// Third party
#include <gtest/gtest.h>

typedef unsigned char uint8_t;

TEST(xD, Base) {
  // Filter
  float const filter2D[3][3] = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
  };
  float filter1D[sizeof filter2D];

  // Image
  const int ROWS = 7;
  const int COLUMNS = 9;
  uint8_t h_image2D[ROWS][COLUMNS];
  uint8_t h_image1D[sizeof h_image2D];
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      h_image2D[r][c] = c;
      h_image1D[r * COLUMNS + c] = c;
    }
  }

  EXPECT_EQ(h_image2D[4][3], h_image1D[4 * COLUMNS + 3]);


}