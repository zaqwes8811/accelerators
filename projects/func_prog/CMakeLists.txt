# debig troubles
# http://stackoverflow.com/questions/19912648/cmake-qtcreator-mingw-debug-mode-windows-7
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# -DCMAKE_BUILD_TYPE=Debug
# -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++

project(func-prog)
cmake_minimum_required(VERSION 2.8)

set(Extern "../../extern/")
set(GMock "../../extern/gmock-1.6.0/fused-src")
set(Canary "./source/canary/")
set(Pro "./source/production/")
set(unitTests "./tests/")
set(Source ".")
set(testsRoot ".")
set(Projects "..")
set(BigExtern "/home/zaqwes/work/big-3rdparty/")
set(Intern "../../intern/")

# Test framework
include_directories(${GMock})
include_directories(${Source})
include_directories(${Extern}/loki-0.1.7/include)
include_directories(${Extern})
include_directories(${BigExtern}/boost_1_57_0)
include_directories(${Projects})
include_directories(${Intern})

# link the libraries to the executable
if (MINGW)
  link_directories(${BigExtern}/boost_1_57_0/stage/lib)
  add_definitions(  -Wall -Wno-unused-local-typedefs)# -Wincompatible-pointer-types)
  LINK_LIBRARIES(ws2_32 WSnmp32 boost_system-mgw48-mt-s-1_57)
elseif (MSVC90)
elseif (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        link_directories(${BigExtern}/boost_1_57_0/stage_gcc/lib)
        link_libraries(tbb tbbmalloc boost_system boost_thread pthread rt)  # !! pthread and rt last!!
    else()
        link_directories(${BigExtern}/boost_1_57_0/stage_x64/lib)
        link_libraries(tbb tbbmalloc boost_system boost_thread pthread rt)  # !! pthread and rt last!!
    endif()
endif()

IF(LCB_USE_ASAN)
    # main: address - thread, memory and ub not work in i686, only x86_64
    #-fno-omit-frame-pointer -O1
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g  -fsanitize=address")
    #-fno-omit-frame-pointer -O1
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g  -fsanitize=address")
    # -undefined dynamic_lookup
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fsanitize=address")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    add_definitions(  -Wall -g -fsanitize=address)
endif()

# Sources
file( GLOB proTests
  ${testsRoot}/david_sankel/*.cc
)

add_executable(unittest.bin
    ${Source}/launcher_tests.cc
    ${GMock}/gmock-gtest-all.cc
    ${proTests}
)

