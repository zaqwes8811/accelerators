################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/Solutions/chapter07/nbody_intrinsic.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter07/nbody_reduction.cu 

C_SRCS += \
../external/book-professional-cuda-c-programming/Solutions/chapter07/closestFP.c 

OBJS += \
./external/book-professional-cuda-c-programming/Solutions/chapter07/closestFP.o \
./external/book-professional-cuda-c-programming/Solutions/chapter07/nbody_intrinsic.o \
./external/book-professional-cuda-c-programming/Solutions/chapter07/nbody_reduction.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/Solutions/chapter07/nbody_intrinsic.d \
./external/book-professional-cuda-c-programming/Solutions/chapter07/nbody_reduction.d 

C_DEPS += \
./external/book-professional-cuda-c-programming/Solutions/chapter07/closestFP.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/Solutions/chapter07/%.o: ../external/book-professional-cuda-c-programming/Solutions/chapter07/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/Solutions/chapter07" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

external/book-professional-cuda-c-programming/Solutions/chapter07/%.o: ../external/book-professional-cuda-c-programming/Solutions/chapter07/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/Solutions/chapter07" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


