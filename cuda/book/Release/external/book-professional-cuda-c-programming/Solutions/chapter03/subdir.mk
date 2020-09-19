################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/Solutions/chapter03/nestedHelloWorldLimited.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter03/nestedHelloWorldNew.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter03/reduceFloatGpu.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-16.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-8-new.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-sync.cu 

C_SRCS += \
../external/book-professional-cuda-c-programming/Solutions/chapter03/reduceFloat.c 

OBJS += \
./external/book-professional-cuda-c-programming/Solutions/chapter03/nestedHelloWorldLimited.o \
./external/book-professional-cuda-c-programming/Solutions/chapter03/nestedHelloWorldNew.o \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceFloat.o \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceFloatGpu.o \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-16.o \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-8-new.o \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-sync.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/Solutions/chapter03/nestedHelloWorldLimited.d \
./external/book-professional-cuda-c-programming/Solutions/chapter03/nestedHelloWorldNew.d \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceFloatGpu.d \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-16.d \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-8-new.d \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceInteger-sync.d 

C_DEPS += \
./external/book-professional-cuda-c-programming/Solutions/chapter03/reduceFloat.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/Solutions/chapter03/%.o: ../external/book-professional-cuda-c-programming/Solutions/chapter03/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/Solutions/chapter03" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

external/book-professional-cuda-c-programming/Solutions/chapter03/%.o: ../external/book-professional-cuda-c-programming/Solutions/chapter03/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/Solutions/chapter03" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


