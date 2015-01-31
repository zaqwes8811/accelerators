// C++
#include <iostream>
#include <string>

// Other
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
  // Run
  testing::InitGoogleTest(&argc, argv);
  testing::GTEST_FLAG(print_time) = true;
  return RUN_ALL_TESTS();
}
