################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter01/hello.cu 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter01/hello.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter01/hello.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter01/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter01/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter01" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


