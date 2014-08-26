#include "addons.h"
///
int PRNG(int &seed, int a, int b, int m) {
  return (a * seed + b) % m;
}


/// разбор ///
void parse(char *s, int& M, int& N, 
  int& a, int& b, int& m, int& seed0) {
  // длина буффера не известна
  int pos_rd = 0;
  int pos_wr = 0;
  int len = strlen(s);

  /// ///
  int rez[6];  // массив с результатом
  int pos_rez = 0;
  
  /// сама разборка ///
  while(pos_rd < len+1) {
    char temp[16];  // пусть здесь, в принципе не критично
    if((s[pos_rd] != ' ') && (s[pos_rd] != 0)) {
      temp[pos_wr] = s[pos_rd];
      pos_wr++;
    }
    else { 
      temp[pos_wr] = 0; pos_wr = 0; 
      // обработка
      rez[pos_rez] = atoi(temp);
      pos_rez++;
    }
    pos_rd++;
  }

  /// разводка ///
  M = rez[0];  // строк
  N = rez[1];
  a = rez[2];
  b = rez[3];
  m = rez[4];
  seed0 = rez[5];
}

/// заполнение ///
void fill(int **A, int& M, int& N, 
  int& a, int& b, int& m, int& seed0) {
  // filling
  int i, j;
  int sum = 0;
  int seed = seed0;
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      seed = PRNG(seed, a, b, m);
      A[i][j] = seed;
      sum = sum + A[i][j];
    }
  }
  int mean = ((float)sum / (M*N)) + 0.5;
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      A[i][j] = A[i][j]-mean;
    }
  }
}
