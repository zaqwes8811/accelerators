

#buildroot-2014.11
RPI_ROOT=/mnt/emb/emb-rpi/output
CCPREFIX=${RPI_ROOT}/host/usr/bin/arm-buildroot-linux-uclibcgnueabi-
# KERNEL_SRC=${RPI_ROOT}/build/linux-c256eb9968c8997dce47350d2075e42f1b3991d3
CC=${CCPREFIX}gcc

# default:
# 	make ARCH=arm CROSS_COMPILE=${CCPREFIX} -C ${KERNEL_SRC} M=$(PWD) modules
# 	$(CC) testebbchar.c -o test

all:
	# -march=armv7-a
	$(CC) -std=c99 -mfloat-abi=softfp -mfpu=neon neon.c -o neon

clean: 
	rm *.ko *.o *.mod.c *.order *.symvers neon