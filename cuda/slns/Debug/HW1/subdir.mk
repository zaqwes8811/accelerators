################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
/home/zaqwes/work/parallelize/third_party/cs344/HW1/hw1_kernels.cu 

CU_DEPS += \
./HW1/hw1_kernels.d 

OBJS += \
./HW1/hw1_kernels.o 


# Each subdirectory must supply rules for building sources it contributes
HW1/hw1_kernels.o: /home/zaqwes/work/parallelize/third_party/cs344/HW1/hw1_kernels.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -I/home/zaqwes/work/opencv_2.3.1/include -I/home/zaqwes/work/parallelize/cuda/src -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "HW1" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc --compile -G -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -I/home/zaqwes/work/opencv_2.3.1/include -I/home/zaqwes/work/parallelize/cuda/src -O0 -g -gencode arch=compute_20,code=compute_20 -gencode arch=compute_20,code=sm_21  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


