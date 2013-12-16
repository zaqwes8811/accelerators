

// Third party
#include <gtest/gtest.h>
#include "cs344/summary/reference_calc.h"

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
  const size_t ROWS = 7;
  const size_t COLUMNS = 9;

  uint8_t h_image2D[ROWS][COLUMNS];
  uint8_t h_InImage1D[sizeof h_image2D];

  uint8_t h_Outimage1D[sizeof h_image2D];

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      h_image2D[r][c] = c;
      h_InImage1D[r * COLUMNS + c] = c;
    }
  }

  EXPECT_EQ(h_image2D[4][3], h_InImage1D[4 * COLUMNS + 3]);

  // —читаем через ref-функцию

}