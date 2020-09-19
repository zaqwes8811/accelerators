################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.config.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.constant.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.flexible.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.legacy.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.openmp.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.overlap.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.parallelized.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-hazards.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-segfault.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-segfault.fixed.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/sumMatrixGPU.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/sumMatrixGPU_nvToolsExt.cu 

C_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.c \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/generate_data.c \
../external/book-professional-cuda-c-programming/CodeSamples/chapter10/generate_userkey.c 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.config.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.constant.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.flexible.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.legacy.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.openmp.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.overlap.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.parallelized.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-hazards.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-segfault.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-segfault.fixed.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/generate_data.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/generate_userkey.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/sumMatrixGPU.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/sumMatrixGPU_nvToolsExt.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.config.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.constant.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.flexible.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.legacy.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.openmp.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.overlap.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.parallelized.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-hazards.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-segfault.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/debug-segfault.fixed.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/sumMatrixGPU.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/sumMatrixGPU_nvToolsExt.d 

C_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/crypt.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/generate_data.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter10/generate_userkey.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter10/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter10/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter10" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

external/book-professional-cuda-c-programming/CodeSamples/chapter10/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter10/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter10" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


