################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src/gmock-gtest-all.cc 

OBJS += \
./third_party/gmock-gtest-all.o 

CC_DEPS += \
./third_party/gmock-gtest-all.d 


# Each subdirectory must supply rules for building sources it contributes
third_party/gmock-gtest-all.o: /home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src/gmock-gtest-all.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -I/home/zaqwes/work/opencv_2.3.1/include -I/home/zaqwes/work/parallelize/cuda/src -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "third_party" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -I/home/zaqwes/work/opencv_2.3.1/include -I/home/zaqwes/work/parallelize/cuda/src -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


