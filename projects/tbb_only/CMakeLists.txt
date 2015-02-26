# debig troubles
# http://stackoverflow.com/questions/19912648/cmake-qtcreator-mingw-debug-mode-windows-7
# http://www.cmake.org/Wiki/CMake_Useful_Variables
#
# -DCMAKE_BUILD_TYPE=Debug
# -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#
# Toolchains:
#   -DCMAKE_TOOLCHAIN_FILE=/home/zaqwes/work/algorithmic-space/platforms/llvm-clang-cilkplus.toolchain.cmake
#
# x64:
#   http://stackoverflow.com/questions/11944060/how-to-detect-target-architecture-using-cmake
#
#
# Intel TBB
#   http://stackoverflow.com/questions/10726537/how-to-install-tbb-from-source-on-linux-and-make-it-work

project(tbb-only)
cmake_minimum_required(VERSION 2.8)

set(Extern "../../extern/")
set(GMock "../../extern/gmock-1.6.0/fused-src")
set(Source "source/")
set(testsRoot "./tests")
set(BigExtern "/home/zaqwes/work/big-3rdparty/")
set(Intern "./intern/")


# Test framework
include_directories(${GMock})
include_directories(${Source})
include_directories(${Extern}/loki-0.1.7/include)
include_directories(${Extern})
include_directories(${BigExtern}/boost_1_57_0)
include_directories(${BigExtern}/asl_1.0.43)
include_directories(${unitTests})
include_directories(${User}/opt/clang-cilkplus/include)
include_directories(${GMock}/)
include_directories(./src/)

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Architecture: ${ARCHITECTURE}" )

# link the libraries to the executable
if (UNIX)
    # gcc 4.8 ubuntu 12 http://llvm.org/docs/GettingStarted.html#getting-a-modern-host-c-toolchain
    # cmake .. -DCMAKE_CXX_LINK_FLAGS="-Wl,-rpath,$HOME/toolchains/lib64 -L$HOME/toolchains/lib64"
    # http://llvm.org/docs/GettingStarted.html#getting-a-modern-host-c-toolchain
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        link_libraries(tbb tbbmalloc pthread rt)  # !! pthread and rt last!!
    else()
        link_directories(${BigExtern}/boost_1_57_0/stage_x64/lib)
        link_libraries(
            tbb tbbmalloc
            boost_system boost_thread
            pthread rt)  # !! pthread and rt last!!
    endif()
endif()

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

# Sources

file(GLOB blockedTestSources
    ${testsRoot}/*.cc)

add_executable(unittest.bin
    ${Source}/mains/launcher_tests.cc
    ${GMock}/gmock-gtest-all.cc
    ${blockedTestSources}
)

