################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter06/asyncAPI.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleCallback.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqBreadth.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqDependence.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqDepth.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqOpenmp.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleMultiAddBreadth.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleMultiAddDepth.cu 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/asyncAPI.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleCallback.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqBreadth.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqDependence.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqDepth.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqOpenmp.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleMultiAddBreadth.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleMultiAddDepth.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/asyncAPI.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleCallback.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqBreadth.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqDependence.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqDepth.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleHyperqOpenmp.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleMultiAddBreadth.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter06/simpleMultiAddDepth.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter06/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter06/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter06" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


