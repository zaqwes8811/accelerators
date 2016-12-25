

# sample:
# http://peterdn.com/post/an-introduction-to-ARM-NEON.aspx
#
# sample:
# asm vs C
# http://hilbert-space.de/?p=22
#
# sample:
# fir - http://www.cnx-software.com/2011/11/27/arm-neon-tutorial-in-c-and-assembler/

# sample:
# big enough: http://armneon.blogspot.ru/2013/07/neon-tutorial-part-1-simple-function_13.html
# with deep optimize
# fixme: may link to C ?

# Tools:
# Pulsar:
# http://pulsar.webshaker.net/ccc/result.php?lng=us
# # -> @

ROOT=/home/zaqwes/tools/buildroot-2013.02-opencv/output
MY_PREFIX=$(ROOT)/host/usr/bin/arm-linux-gnueabihf-

all:
	$(MY_PREFIX)gcc -g neon.s -o neon
	#$(MY_PREFIX)objdump -d neon


from_c:
	$(MY_PREFIX)gcc -S neon_c.c

clean:
	rm neon neon_c neon_c.s