################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
/home/zaqwes/work/parallelize/cuda/src/reuse.cu \
/home/zaqwes/work/parallelize/cuda/src/xd.cu 

CC_SRCS += \
/home/zaqwes/work/parallelize/cuda/src/hw1_test.cc \
/home/zaqwes/work/parallelize/cuda/src/hw2_test.cc \
/home/zaqwes/work/parallelize/cuda/src/launcher.cc \
/home/zaqwes/work/parallelize/cuda/src/opencv_test_app.cc \
/home/zaqwes/work/parallelize/cuda/src/splitters.cc \
/home/zaqwes/work/parallelize/cuda/src/splitters_test.cc \
/home/zaqwes/work/parallelize/cuda/src/xD_test.cc 

CU_DEPS += \
./reuse.d \
./xd.d 

OBJS += \
./hw1_test.o \
./hw2_test.o \
./launcher.o \
./opencv_test_app.o \
./reuse.o \
./splitters.o \
./splitters_test.o \
./xD_test.o \
./xd.o 

CC_DEPS += \
./hw1_test.d \
./hw2_test.d \
./launcher.d \
./opencv_test_app.d \
./splitters.d \
./splitters_test.d \
./xD_test.d 


# Each subdirectory must supply rules for building sources it contributes
hw1_test.o: /home/zaqwes/work/parallelize/cuda/src/hw1_test.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

hw2_test.o: /home/zaqwes/work/parallelize/cuda/src/hw2_test.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

launcher.o: /home/zaqwes/work/parallelize/cuda/src/launcher.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

opencv_test_app.o: /home/zaqwes/work/parallelize/cuda/src/opencv_test_app.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

reuse.o: /home/zaqwes/work/parallelize/cuda/src/reuse.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc --compile -G -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -O0 -g -gencode arch=compute_20,code=compute_20 -gencode arch=compute_20,code=sm_21  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

splitters.o: /home/zaqwes/work/parallelize/cuda/src/splitters.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

splitters_test.o: /home/zaqwes/work/parallelize/cuda/src/splitters_test.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

xD_test.o: /home/zaqwes/work/parallelize/cuda/src/xD_test.cc
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

xd.o: /home/zaqwes/work/parallelize/cuda/src/xd.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-5.5/bin/nvcc -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -G -g -O0 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-5.5/bin/nvcc --compile -G -I/home/zaqwes/work/parallelize/third_party/gmock-1.6.0/fused-src -I/home/zaqwes/work/parallelize/third_party -O0 -g -gencode arch=compute_20,code=compute_20 -gencode arch=compute_20,code=sm_21  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


