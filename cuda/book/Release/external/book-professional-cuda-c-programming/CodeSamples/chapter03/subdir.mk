################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedHelloWorld.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduce.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduce2.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduceNosync.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter03/reduceInteger.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter03/simpleDeviceQuery.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter03/simpleDivergence.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter03/sumMatrix.cu 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedHelloWorld.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduce.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduce2.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduceNosync.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/reduceInteger.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/simpleDeviceQuery.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/simpleDivergence.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/sumMatrix.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedHelloWorld.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduce.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduce2.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/nestedReduceNosync.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/reduceInteger.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/simpleDeviceQuery.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/simpleDivergence.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter03/sumMatrix.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter03/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter03/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter03" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


