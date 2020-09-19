################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/globalVariable.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/memTransfer.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/pinMemTransfer.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/readSegment.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/readSegmentUnroll.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/simpleMathAoS.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/simpleMathSoA.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumArrayZerocpy.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumMatrixGPUManaged.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumMatrixGPUManual.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/transpose.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter04/writeSegment.cu 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/globalVariable.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/memTransfer.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/pinMemTransfer.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/readSegment.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/readSegmentUnroll.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/simpleMathAoS.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/simpleMathSoA.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumArrayZerocpy.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumMatrixGPUManaged.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumMatrixGPUManual.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/transpose.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/writeSegment.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/globalVariable.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/memTransfer.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/pinMemTransfer.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/readSegment.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/readSegmentUnroll.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/simpleMathAoS.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/simpleMathSoA.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumArrayZerocpy.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumMatrixGPUManaged.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/sumMatrixGPUManual.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/transpose.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter04/writeSegment.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter04/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter04/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter04" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


