

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

/// TROUBLE: align
//   http://www.cplusplus.com/forum/general/98106/
//   https://msdn.microsoft.com/en-us/library/01fth20w(v=vs.80).aspx

// __attribute__(aligned(16)) 
struct Vector4
{    
	Vector4() {
		x = y = z = w = 0;
	}

    float x, y, z, w;        
};

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

Vector4 SSE_mul( const Vector4& Op_A, const Vector4& Op_B )
{
	// __m128 vector1 = _mm_set1_ps(4, 3, 2, 1); // Little endian, stored in 'reverse'
	Vector4 Ret_Vector;
	// __m128 f = _mm_set1_ps( Op_B );
	__m128 f = _mm_loadu_ps( (float*)&Op_A );

	return Ret_Vector;
}

int main()
{
	// похоже это просто хранилища
	// AlignedStorage<Vector4, 16> Op_C;
	// AlignedStorage<Vector4, 16> Op_D;

	// cout << hex << Op_C.data << " " << Op_D << endl;
	Vector4 Op_A, Op_B;

	// align
	// https://software.intel.com/en-us/forums/intel-c-compiler/topic/328019
	//__attribute__ ((aligned(16)))
	Op_A.x = 1;
	Op_B.x = 2;
	Vector4 r = 
		// SSE_Add
		SSE_mul
			( Op_A, Op_B ); 
	// assert( ( r.x - 3 ) < 1e-5 );
	return 0;
}