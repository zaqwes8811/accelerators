################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter05/checkSmemRectangle.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter05/checkSmemSquare.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter05/constantReadOnly.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter05/constantStencil.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter05/reduceInteger.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter05/reduceIntegerShfl.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter05/simpleShfl.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter05/transposeRectangle.cu 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/checkSmemRectangle.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/checkSmemSquare.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/constantReadOnly.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/constantStencil.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/reduceInteger.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/reduceIntegerShfl.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/simpleShfl.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/transposeRectangle.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/checkSmemRectangle.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/checkSmemSquare.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/constantReadOnly.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/constantStencil.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/reduceInteger.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/reduceIntegerShfl.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/simpleShfl.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter05/transposeRectangle.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter05/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter05/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter05" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


