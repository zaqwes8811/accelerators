 // file : include "addons.h"
#ifndef ADD_H_GUARD
#define ADD_H_GUARD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// вычислительные
void findCorners(int **A);  // главная

// вспомогательные
int PRNG(int &seed, int a, int b, int m);
void parse(char *s, int& M, int& N, 
  int& a, int& b, int& m, int& seed0);  // дробит полученную из файла строку
void fill(int **A, int& M, int& N, 
  int& a, int& b, int& m, int& seed0);
#endif
