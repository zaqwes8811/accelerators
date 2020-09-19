################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/Solutions/chapter09/simple2DFDModified.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter09/simpleMultiGPUEvents-initial.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter09/simpleMultiGPUEvents.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_PingPong.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_PingPongDefault.cu 

C_SRCS += \
../external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P-async.c \
../external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_Pageable.c 

OBJS += \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simple2DFDModified.o \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleMultiGPUEvents-initial.o \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleMultiGPUEvents.o \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P-async.o \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_Pageable.o \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_PingPong.o \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_PingPongDefault.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simple2DFDModified.d \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleMultiGPUEvents-initial.d \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleMultiGPUEvents.d \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_PingPong.d \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_PingPongDefault.d 

C_DEPS += \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P-async.d \
./external/book-professional-cuda-c-programming/Solutions/chapter09/simpleP2P_Pageable.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/Solutions/chapter09/%.o: ../external/book-professional-cuda-c-programming/Solutions/chapter09/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/Solutions/chapter09" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

external/book-professional-cuda-c-programming/Solutions/chapter09/%.o: ../external/book-professional-cuda-c-programming/Solutions/chapter09/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/Solutions/chapter09" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


