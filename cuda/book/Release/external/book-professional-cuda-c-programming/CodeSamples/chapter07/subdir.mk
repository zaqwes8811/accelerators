################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter07/atomic-ordering.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter07/floating-point-accuracy.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter07/floating-point-perf.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter07/fmad.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter07/intrinsic-standard-comp.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter07/my-atomic-add.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter07/nbody.cu 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/atomic-ordering.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/floating-point-accuracy.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/floating-point-perf.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/fmad.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/intrinsic-standard-comp.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/my-atomic-add.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/nbody.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/atomic-ordering.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/floating-point-accuracy.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/floating-point-perf.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/fmad.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/intrinsic-standard-comp.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/my-atomic-add.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter07/nbody.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter07/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter07/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter07" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


