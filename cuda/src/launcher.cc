
// Other
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
  // Run
  testing::InitGoogleTest(&argc, argv);
  testing::GTEST_FLAG(print_time) = true;
  RUN_ALL_TESTS();
	system("pause");
  return 0;
}

