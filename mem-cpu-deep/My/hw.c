#include <stdio.h>
#include <stdint.h>

int kern(float* p, int32_t* k){
	p[9] = 10;
	return 0;}

int main( )
{ 
  printf("Hello World!\n");
}
