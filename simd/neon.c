

#ifdef __ARM_NEON__
#include <arm_neon.h>
#endif

#include <stdint.h>

// >= armv7

// "Copying from a NEON register to a CPU ..."
// "Think of NEON like a co-processor"

void threshold(unsigned char* src, unsigned char* dst, 
	int length,	unsigned char thresh, unsigned char maxval)
{
	for(int i = 0; i < length; ++i)
		dst[i] = src[i] <= thresh ? 0 : maxval;
}

// fixme: нужно ли выравнивать?
void threshold_NEON(unsigned char* src, unsigned char* dst, int length, 
	unsigned char thresh, unsigned char maxval)
{

	// names: line, vector, scalar
	// "32 registers, 64-bits wide 
	//   (dual view as 16 registers, 128-bits wide)"
	// 16 чаров
	// q - 128 bit vector of
	uint8x16_t vthreshold = vdupq_n_u8( thresh );
	uint8x16_t vvalue     = vdupq_n_u8( maxval );
	for(int i = 0; i <= length - 32; i += 32 )
	{
		// обрабатываем подва

		// подгружаем в кеш?
		__builtin_prefetch(src + i + 320);  // fixme: почему 320?
		uint8x16_t v0 = vld1q_u8(src + i);
		uint8x16_t v1 = vld1q_u8(src + i + 16);

		// обрабатываем
		// compare greater then = cgt
		uint8x16_t r0 = vcgtq_u8(v0, vthreshold);
		uint8x16_t r1 = vcgtq_u8(v1, vthreshold);

		// почему and?
		uint8x16_t r0a = vandq_u8(r0, vvalue);
		uint8x16_t r1a = vandq_u8(r1, vvalue);
		vst1q_u8(dst + i, r0a);
		vst1q_u8(dst + i + 16, r1a);
	}
}

// fixme: bgra unpack

int main()
{

	// http://www.ethernut.de/en/documents/arm-inline-asm.html
	// diff from x86
	int a = 90;
	__asm__
	(
		"mov r0, %[a];"::[a] "r" (a)
	);
	return 0;
}