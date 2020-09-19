################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkDeviceInfor.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkDimension.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkThreadIndex.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/defineGridBlock.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnGPU-small-case.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnGPU-timer.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-1D-grid-1D-block.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-2D-grid-1D-block.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-2D-grid-2D-block.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU.cu 

C_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnHost.c 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkDeviceInfor.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkDimension.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkThreadIndex.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/defineGridBlock.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnGPU-small-case.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnGPU-timer.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnHost.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-1D-grid-1D-block.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-2D-grid-1D-block.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-2D-grid-2D-block.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkDeviceInfor.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkDimension.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/checkThreadIndex.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/defineGridBlock.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnGPU-small-case.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnGPU-timer.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-1D-grid-1D-block.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-2D-grid-1D-block.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU-2D-grid-2D-block.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumMatrixOnGPU.d 

C_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter02/sumArraysOnHost.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter02/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter02/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter02" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

external/book-professional-cuda-c-programming/CodeSamples/chapter02/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter02/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter02" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


