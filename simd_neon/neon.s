# gas uses AT&T syntax:
#mnemonic    source, destination

# !!! Intex Syntax
#nasm uses intel style:
#mnemonic    destination, source

# http://www.toves.org/books/arm/


	# .align	2  # fixme: compiled but...
	.global	main
	# .thumb
	# .thumb_func
	# .type	main, %function
main:
	# @ args = 0, pretend = 0, frame = 0
	# @ frame_needed = 1, uses_anonymous_args = 0
	# @ link register save eliminated.
	# Neon
	# r0 - ?
	# [r0] - *src ??
	# d0-2 - 8*8=64 bit regs
	vld3.8 {d0, d1, d2}, [r0]
	# load (vld) or store (vst)
	# interleave pattern (1, 2, 3 or 4)
	# element type (8, 16, 32)
	# addr in arm reg
	# "Finally, element size has an impact on pointer alignment. 
	# Alignment to the element size will generally give better 
	# performance, and it may be a requirement of your target operating 
	# system."
	# fixme: how align?
	# "When aligning memory accesses for speed, remember to 
	# use :64 or :128 or :256 address qualifiers with your load and 
	# store instructions, for optimum performance."

	# one elem
	# [] -> [idx] for scutter
	vld3.8 {d0[], d1[], d2[]}, [r0]  

	@ Trouble:
	@ leftover element - len array = 21

	@ Matrix mult
	@
	@ On same hardware - ways usige
	@ 32 - double word 64 bits - d0 - d31
	@ 16 quad-word reg - 128 bit named q0-q15

	@ Trouble: 
	@ Scheduling

	# Some stuff
	push	{r7}
	add	r7, sp, #0
	movs	r3, #0
	mov	r0, r3
	mov	sp, r7
	pop	{r7}
	bx	lr



	# .size	main, .-main
	# .ident	"GCC: (crosstool-NG linaro-1.13.1-4.7-2013.01-20130125 - Linaro GCC 2013.01) 4.7.3 20130102 (prerelease)"
	# .section	.note.GNU-stack,"",%progbits
