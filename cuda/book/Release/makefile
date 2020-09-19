################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include external/book-professional-cuda-c-programming/Solutions/chapter09/subdir.mk
-include external/book-professional-cuda-c-programming/Solutions/chapter08/subdir.mk
-include external/book-professional-cuda-c-programming/Solutions/chapter07/subdir.mk
-include external/book-professional-cuda-c-programming/Solutions/chapter05/subdir.mk
-include external/book-professional-cuda-c-programming/Solutions/chapter04/subdir.mk
-include external/book-professional-cuda-c-programming/Solutions/chapter03/subdir.mk
-include external/book-professional-cuda-c-programming/Solutions/chapter02/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/lectures_code/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/lectures_code/iso/src/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter10/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter09/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter08/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter07/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter06/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter05/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter04/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter03/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter02/subdir.mk
-include external/book-professional-cuda-c-programming/CodeSamples/chapter01/subdir.mk

OS_SUFFIX := $(subst Linux,linux,$(subst Darwin/x86_64,darwin,$(shell uname -s)/$(shell uname -m)))

-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CU_DEPS)),)
-include $(CU_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# OpenGL+GLUT OS-specific define
ifeq ($(shell uname -s),Darwin)
GLUT_LIBS := -Xlinker -framework -Xlinker GLUT -Xlinker -framework -Xlinker OpenGL
else
GLUT_LIBS := -lGL -lGLU -lglut 
endif


# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: book

# Tool invocations
book: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: NVCC Linker'
	/usr/bin/nvcc --cudart static --relocatable-device-code=false -gencode arch=compute_30,code=compute_30 -gencode arch=compute_30,code=sm_30 -link -o  "book" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CU_DEPS)$(CPP_DEPS)$(C_DEPS) book
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
