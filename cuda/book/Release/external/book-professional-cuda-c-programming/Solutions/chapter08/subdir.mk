################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../external/book-professional-cuda-c-programming/Solutions/chapter08/access-ordering.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter08/cublas-matrix-matrix-async.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter08/cublas-matrix-matrix.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter08/cufft-inverse.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter08/cusparse-matrix-matrix-double.cu \
../external/book-professional-cuda-c-programming/Solutions/chapter08/cusparse-matrix-matrix.cu 

OBJS += \
./external/book-professional-cuda-c-programming/Solutions/chapter08/access-ordering.o \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cublas-matrix-matrix-async.o \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cublas-matrix-matrix.o \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cufft-inverse.o \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cusparse-matrix-matrix-double.o \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cusparse-matrix-matrix.o 

CU_DEPS += \
./external/book-professional-cuda-c-programming/Solutions/chapter08/access-ordering.d \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cublas-matrix-matrix-async.d \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cublas-matrix-matrix.d \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cufft-inverse.d \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cusparse-matrix-matrix-double.d \
./external/book-professional-cuda-c-programming/Solutions/chapter08/cusparse-matrix-matrix.d 


# Each subdirectory must supply rules for building sources it contributes
external/book-professional-cuda-c-programming/Solutions/chapter08/%.o: ../external/book-professional-cuda-c-programming/Solutions/chapter08/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_30,code=sm_30  -odir "external/book-professional-cuda-c-programming/Solutions/chapter08" -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


