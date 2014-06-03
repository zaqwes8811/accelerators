############################################################################
# <summary> CMakeLists.txt for OpenCV and CUDA. </summary>
# <date>    2012-02-07          </date>
# <author>  Quan Tran Minh. edit by Johannes Kast, Michael Sarahan </author>
# <email>   quantm@unist.ac.kr  kast.jo@googlemail.com msarahan@gmail.com</email>
############################################################################

# http://stackoverflow.com/questions/13073717/building-cuda-object-files-using-cmake

# required cmake version                                                                                                                                                                                   
cmake_minimum_required(VERSION 2.8)

# packages                                                                                                                                                                                                 
find_package(CUDA)

include_directories(/home/zaqwes/work/opencv_2.3.1/include)

# nvcc flags                                                                                                                                                                                               
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_20,code=sm_20)


SET (compare_files compare.cpp)

file( GLOB  hdr *.hpp *.h )
file( GLOB  cu  *.cu)
SET (HW3_files main.cpp loadSaveImage.cpp reference_calc.cpp compare.cpp)
    
CUDA_ADD_EXECUTABLE(HW3 ${HW3_files} ${hdr} ${cu})
