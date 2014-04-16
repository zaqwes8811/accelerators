################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/zaqwes/work/parallelize/third_party/cs344/reuse/compare.cpp 

CC_SRCS += \
/home/zaqwes/work/parallelize/third_party/cs344/reuse/utils.cc 

OBJS += \
./third_party/reuse/compare.o \
./third_party/reuse/utils.o 

CC_DEPS += \
./third_party/reuse/utils.d 

CPP_DEPS += \
./third_party/reuse/compare.d 


# Each subdirectory must supply rules for building sources it contributes
third_party/reuse/compare.o: /home/zaqwes/work/parallelize/third_party/cs344/reuse/compare.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -I/home/zaqwes/work/opencv_2.3.1/include -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "third_party/reuse" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -I/home/zaqwes/work/opencv_2.3.1/include -G -g -O0   "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

third_party/reuse/utils.o: /home/zaqwes/work/parallelize/third_party/cs344/reuse/utils.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -I/home/zaqwes/work/opencv_2.3.1/include -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "third_party/reuse" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -I/home/zaqwes/work/opencv_2.3.1/include -G -g -O0   "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


