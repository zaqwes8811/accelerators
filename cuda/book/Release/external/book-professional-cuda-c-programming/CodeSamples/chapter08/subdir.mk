################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/cublas.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/cuda-openacc.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/cufft-multi.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/cufft.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/cusparse.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/rand-kernel.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/replace-rand-streams.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/replace-rand.cu 

C_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/drop-in.c \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-data.c \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-kernels.c \
../external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-parallel.c 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cublas.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cuda-openacc.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cufft-multi.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cufft.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cusparse.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/drop-in.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/rand-kernel.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/replace-rand-streams.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/replace-rand.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-data.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-kernels.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-parallel.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cublas.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cuda-openacc.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cufft-multi.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cufft.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/cusparse.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/rand-kernel.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/replace-rand-streams.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/replace-rand.d 

C_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/drop-in.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-data.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-kernels.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter08/simple-parallel.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter08/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter08/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter08" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

external/book-professional-cuda-c-programming/CodeSamples/chapter08/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter08/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter08" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


