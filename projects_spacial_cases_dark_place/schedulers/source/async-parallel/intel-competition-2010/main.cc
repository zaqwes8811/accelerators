#include "addons.h"
#include <fstream>
#include <iostream>
/*
  1 ≤ C ≤ 1000
  1 ≤ M, N, a, b, m, seed0 ≤ 20000
*/
using namespace std;

/// ///
int main(int argc, const char *argv[]) {
	int **A;  // исходная матрица
	// читаем данные из файла
  ifstream input(argv[1]);
  if(!input) {
    cout<<"Ошибка открытия файла"<<endl;
	  return 1;

  }

  const int bLen = 64; char s[bLen];
  input.getline(s, bLen);
  int C = atoi(s);  // число тестов

  /// зацикливаем по числу испытаний///
  for(int k = 0; k < C; k++) {
    int M = 0; 
    int N = 0;
    int a, b, m, seed0;
    // разборка строки и генерация матрицы
    input.getline(s, bLen);
    cout<<"\nВходные параметры : "<<s<<endl;

    parse(s, M, N, a, b, m, seed0);  // разбираем строку файла
    // выделение памяти - нужно проверить выделяет ли!
    A = new int *[M];
    for(int i = 0; i < M; i++)
      A[i] = new int[N];
    
    // заполняем
    fill(A, M, N, a, b, m, seed0);
  

    // выводе сгенерированной матрицы
    for (int i = 0; i < M; i++) {
      cout<<endl;
      for (int j = 0; j < N; j++) {
         cout<<" "<<A[i][j];
      }
    }
    cout<<endl;
    /// /// /// ///

    


    /// /// /// ///
    // освобождение памяти
    for(int i = 0; i < M; i++)
       if(A[i] != NULL) delete [] A[i];
    if(A != NULL) delete [] A;
  }
  input.close();
  return 0; 
}



