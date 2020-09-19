################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter09/simple2DFD.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleMultiGPU.cu \
../external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P_PingPong.cu 

C_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleC2C.c \
../external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P.c \
../external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P_CUDA_Aware.c 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simple2DFD.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleC2C.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleMultiGPU.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P_CUDA_Aware.o \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P_PingPong.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simple2DFD.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleMultiGPU.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P_PingPong.d 

C_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleC2C.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P.d \
./external/book-professional-cuda-c-programming/CodeSamples/chapter09/simpleP2P_CUDA_Aware.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/chapter09/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter09/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter09" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

external/book-professional-cuda-c-programming/CodeSamples/chapter09/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/chapter09/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/chapter09" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


