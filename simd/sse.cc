

#include "iostream"
#include <type_traits>

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <xmmintrin.h>

using namespace std;

//http://stackoverflow.com/questions/5134217/aligning-data-on-the-stack-c
template <class T, int N>
struct AlignedStorage
{
    typename std::aligned_storage<
    	sizeof(T) * N, std::alignment_of<T>::value>::type data;
};


// Getting started
//   http://stackoverflow.com/questions/1389712/getting-started-with-sse

// A 16byte = 128bit vector struct

// http://neilkemp.us/src/sse_tutorial/sse_tutorial.html
// sse - collection 128 bit cpu regis.
// float - 32 bit

//////
// https://www.inf.ethz.ch/personal/markusp/teaching/263-2300-ETH-spring11/slides/class17.pdf
// https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
// asm: http://rayseyfarth.com/asm/pdf/ch11-floating-point.pdf
//   and some !!!tasks.

// asm gdb https://www.recurse.com/blog/7-understanding-c-by-learning-assembly


struct Vector4
{    
	Vector4() {
		x = y = z = w = 0;
	}

    float x, y, z, w;        
} __attribute__((aligned(16)));

Vector4 SSE_Add( const Vector4& Op_A, const Vector4& Op_B ) 
{
	Vector4 Ret_Vector;

	// https://www.recurse.com/blog/7-understanding-c-by-learning-assembly
    /* clobbered register */  // withou segfault if -Ofast
	__asm__ (
		"mov %[a], %%eax;"
		"mov %[b], %%ebx;"
	    "movupd (%%eax), %%xmm0;"
	    "movupd (%%ebx), %%xmm1;"
	    "addps %%xmm0, %%xmm1;"
	    "mov %[r], %%eax;"
	    "movupd %%xmm1, (%%eax);"
	    : : [r] "rm"( &Ret_Vector ), 
	    	[a] "r"( &Op_A ), [b] "r"( &Op_B )
	    :"eax", "ebx", "xmm0", "xmm1", "cc"
	);
	return Ret_Vector;
}

// asm vs intri.
//   https://software.intel.com/en-us/forums/software-tuning-performance-optimization-platform-monitoring/topic/280882
Vector4 SSE_mul( const Vector4& Op_A, const Vector4& Op_B )
{
	// __m128 vector1 = _mm_set1_ps(4, 3, 2, 1); // Little endian, stored in 'reverse'
	Vector4 Ret_Vector;
	// __m128 f = _mm_set1_ps( Op_B );
	__m128 a = _mm_load_ps( (float*)&Op_A );
	__m128 b = _mm_load_ps( (float*)&Op_B );
	__m128 r = _mm_mul_ps( a, b );

	_mm_store_ps((float*)&Ret_Vector, r);

	assert( Op_B.x * 5 == Ret_Vector.x );

	// __asm__(
		//"mov %[f], %%eax;"
		// "mulps %%xmm0, %[f]" :: [f] "m" (f)  // can't make it work
	// );

	return Ret_Vector;
}

int main()
{
	// похоже это просто хранилища
	// AlignedStorage<Vector4, 16> Op_C;
	// AlignedStorage<Vector4, 16> Op_D;

	Vector4 Op_A[2];
	char dummy[17];
	Vector4 Op_B;

	Vector4* v = (Vector4 *)_mm_malloc(sizeof(Vector4), 16);

	cout << hex << &Op_B << endl << &Op_A << endl;

	Op_A[0].x = 5;
	Op_A[1].x = 2;
	Vector4 r = 
		// SSE_Add
		SSE_mul
			( Op_A[0], *v ); 
	// assert( ( r.x - 3 ) < 1e-5 );

	_mm_free( v );
	return 0;
}