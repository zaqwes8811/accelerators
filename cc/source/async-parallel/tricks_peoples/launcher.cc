// C++
// Third party
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{

  // Получаем текущую локаль CRT (если нужно потом восстановить)
  char* crtLocale = setlocale(LC_ALL, NULL);
  setlocale(LC_ALL, ".1251");

  // Run
  testing::InitGoogleTest(&argc, argv);
  //testing::GTEST_FLAG(print_time) = true;
  int result = RUN_ALL_TESTS();
  setlocale(LC_ALL, crtLocale);
  //system("pause");
  return result;
}
