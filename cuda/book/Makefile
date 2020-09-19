
CU_APPS=ch1
C_APPS=

# https://stackoverflow.com/questions/4036191/sources-from-subdirectories-in-makefile
# TODO How to compile *.cu from zero level folder?
# TODO NSight ignores this Makefile

all: 
	#${C_APPS} ${CU_APPS}
	nvcc -O2 -arch=sm_30 -o ch1 ch1.cu -lcudadevrt --relocatable-device-code true

#%: %.cu
#	nvcc -O2 -arch=sm_35 -o $@ $< -lcudadevrt --relocatable-device-code true
#%: %.c
#	gcc -O2 -std=c99 -o $@ $<
clean:
	rm -f ${CU_APPS} ${C_APPS}
