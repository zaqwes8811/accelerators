############################################################################
# <summary> CMakeLists.txt for OpenCV and CUDA. </summary>
# <date>    2012-02-07          </date>
# <author>  Quan Tran Minh. edit by Johannes Kast, Michael Sarahan </author>
# <email>   quantm@unist.ac.kr  kast.jo@googlemail.com msarahan@gmail.com</email>
############################################################################

# http://stackoverflow.com/questions/13073717/building-cuda-object-files-using-cmake
# https://devtalk.nvidia.com/default/topic/502947/cuda-cmake-set_target_properties-error/

# required cmake version                                                                                                                                                                                   
cmake_minimum_required(VERSION 2.8)

# packages                                                                                                                                                                                                 
find_package(CUDA)

# Intall somewhere
#set(Thrust "/home/zaqwes/work/third_party")
#include_directories(${Thrust})

# nvcc flags                                                                                                                                                                                               
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_20,code=sm_20)

file( GLOB  hdr *.hpp *.h )
file( GLOB  cu  *.cu)
# 
set (HW5_files  reference_calc.cpp)
    
CUDA_ADD_EXECUTABLE(a.app ${HW5_files} ${hdr} ${cu})



