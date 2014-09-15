/*
 ============================================================================
 Name        : MyOmp.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * Hello OpenMP World prints the number of threads and the current thread id
 */
int main (int argc, char *argv[]) {

  int numThreads, tid;
  int a, b;

#pragma omp parallel
  {
  a = 1;
    b = 3;
    a = 6;
    b = 8;
}
  tid = omp_get_thread_num();

  tid = omp_get_thread_num();


  /* This creates a team of threads; each thread has own copy of variables  */
#pragma omp parallel private(numThreads, tid)
 {
   tid = omp_get_thread_num();
   printf("Hello World from thread number %d\n", tid);

   /* The following is executed by the master thread only (tid=0) */
   if (tid == 0)
     {
       numThreads = omp_get_num_threads();
       printf("Number of threads is %d\n", numThreads);
     }
 }
 return 0;
}


