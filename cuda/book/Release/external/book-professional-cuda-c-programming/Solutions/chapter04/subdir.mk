################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/Solutions/chapter04/globalVariable1.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/globalVariable2.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/memTransfer.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/pinMemTransfer.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/readWriteSegment.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/readWriteSegmentUnroll.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/simpleMathAoS-align.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/simpleMathAoS-x.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpy-offset.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpyUVA-offset.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpyUVA.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter04/transpose.cu 

OBJS += \
./external/book-professional-cuda-c-programming/Solutions/chapter04/globalVariable1.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/globalVariable2.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/memTransfer.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/pinMemTransfer.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/readWriteSegment.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/readWriteSegmentUnroll.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/simpleMathAoS-align.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/simpleMathAoS-x.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpy-offset.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpyUVA-offset.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpyUVA.o \
./external/book-professional-cuda-c-programming/Solutions/chapter04/transpose.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/Solutions/chapter04/globalVariable1.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/globalVariable2.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/memTransfer.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/pinMemTransfer.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/readWriteSegment.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/readWriteSegmentUnroll.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/simpleMathAoS-align.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/simpleMathAoS-x.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpy-offset.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpyUVA-offset.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/sumArrayZerocpyUVA.d \
./external/book-professional-cuda-c-programming/Solutions/chapter04/transpose.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/Solutions/chapter04/%.o: ../external/book-professional-cuda-c-programming/Solutions/chapter04/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/Solutions/chapter04" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


