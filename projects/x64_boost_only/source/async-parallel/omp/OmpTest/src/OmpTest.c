/*
 ============================================================================
 Name        : OmpTest.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/**
 * Hello OpenMP World prints the number of threads and the current thread id
 */
int main (int argc, char *argv[]) {
int k;
int x;
#pragma omp parallel for private(k, x)
	for(k = 0; k < 80; k++)
	{
		x = sin(k*2.0)*100+1;
		if(x > 60)
			x =  x % 60+1;
		printf("x = %d\n", x);
		int numth = omp_get_num_threads();

		printf("th = %d\n", numth);

	}

  //
  int num = omp_get_num_procs();
  printf("cores = %d\n", num);
  //
 return 0;
}


