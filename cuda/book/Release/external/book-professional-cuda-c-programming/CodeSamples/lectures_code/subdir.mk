################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../external/book-professional-cuda-c-programming/CodeSamples/lectures_code/access-patterns.c \
../external/book-professional-cuda-c-programming/CodeSamples/lectures_code/atomic-demo.c \
../external/book-professional-cuda-c-programming/CodeSamples/lectures_code/loop-schedule.c \
../external/book-professional-cuda-c-programming/CodeSamples/lectures_code/query_openacc_devices.c \
../external/book-professional-cuda-c-programming/CodeSamples/lectures_code/reduction-demo.c 

OBJS += \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/access-patterns.o \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/atomic-demo.o \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/loop-schedule.o \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/query_openacc_devices.o \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/reduction-demo.o 

C_DEPS += \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/access-patterns.d \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/atomic-demo.d \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/loop-schedule.d \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/query_openacc_devices.d \
./external/book-professional-cuda-c-programming/CodeSamples/lectures_code/reduction-demo.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/CodeSamples/lectures_code/%.o: ../external/book-professional-cuda-c-programming/CodeSamples/lectures_code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/CodeSamples/lectures_code" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile  -x c -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


