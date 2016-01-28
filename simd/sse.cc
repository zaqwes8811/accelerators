// http://neilkemp.us/src/sse_tutorial/sse_tutorial.html

// sse - collection 128 bit cpu regis.

// float - 32 bit

// A 16byte = 128bit vector struct
#include <assert.h>
#include <stdint.h>

struct Vector4
{    
        float x, y, z, w;        
};

Vector4 SSE_Add( const Vector4& Op_A, const Vector4& Op_B ) 
{
		// fixme: make asm inserts
	Vector4 Ret_Vector;
	void* p = (void*)&Op_A;
	void* p1 = 0;

	int a, b;
	// __asm__
	// (
	// 	"movl %1, %%eax\n\t"
	// 	"movl %%eax, %0\n\t"
	// 	:"=r" (p1) : "r"(p)
	// );

	int i = 0;
	uint32_t* faulting_address = reinterpret_cast<uint32_t*>(&i);
	asm volatile("mov %%eax, %0" : "=r" (faulting_address));


	// assert( p == p1 );
		// 	mov ebx, Op_B

		// movups xmm0, [eax]
		// movups xmm1, [ebx]

		// addps xmm0, xmm1
		// movups [Ret_Vector], xmm0
}

int main()
{
	Vector4 Op_A, Op_B;
	SSE_Add( Op_A, Op_B ); 


	return 0;
}