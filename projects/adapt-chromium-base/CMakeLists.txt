# -DCMAKE_BUILD_TYPE=Debug
# -DCMAKE_BUILD_TYPE=Release
# -DCMAKE_VERBOSE_MAKEFILE=ON
# -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#
# ASan:
#  https://software.intel.com/en-us/articles/address-sanitizer-asan-tool-to-debug-memory-corruptions-in-chrome-os
#  http://www.youtube.com/watch?v=V2_80g0eOMc
#  http://www.youtube.com/watch?v=capbD_aRz40

project(chromium-base-adapt)
cmake_minimum_required(VERSION 2.8)

#set(Extern "../../extern/")
#set(GMock "../../extern/gmock-1.6.0/fused-src")
set(Source "source/")
set(Tests "./tests")
set(BigExtern "/opt/big-3rdparty/")
set(ChromiumRoot "/home/zaqwes/mnt/ssd_hard/src")
set(ChromiumTesting "/home/zaqwes/mnt/ssd_hard/src/testing")

# conf
set (LCB_USE_TSAN Y)

# Test framework
include_directories(${ChromiumRoot})
include_directories(${ChromiumTesting}/gmock/include)
include_directories(${ChromiumTesting}/gmock)
include_directories(${ChromiumTesting}/gtest/include)
#include_directories(${GMock})
include_directories(${Source})
include_directories(${BigExtern}/boost_1_57_0)

# link the libraries to the executable
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  # need C++11

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        IF(LCB_USE_TSAN)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fsanitize=thread")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -fsanitize=thread")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fsanitize=thread")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
            link_directories(${BigExtern}/boost_1_57_0/clang_tsan_lib64/lib)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -analyse")
        endif()

        link_libraries(
            boost_system boost_thread
            pthread rt)  # !! pthread and rt last!!
    else()
        IF(LCB_USE_TSAN)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -fsanitize=thread -fPIC")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -fsanitize=thread -fPIC")



            #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fsanitize=thread")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread -pie")
            link_directories(${BigExtern}/boost_1_57_0/gcc_tsan_lib64/lib)
        endif()
        link_libraries(
            boost_system boost_thread
            tsan
            pthread rt)  # !! pthread and rt last!!
    endif()
endif()

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
file( GLOB set0
    ${Source}/mains/*.cc)

file( GLOB chromiumTests
${ChromiumRoot}/base/bind_unittest.cc
${ChromiumRoot}/base/memory/ref_counted.cc
${ChromiumRoot}/base/memory/weak_ptr.cc
${ChromiumRoot}/base/callback*.cc
${ChromiumRoot}/base/logging.cc
${ChromiumRoot}/base/*switches.cc
${ChromiumRoot}/base/*command_line.cc
${ChromiumRoot}/base/vlog.cc
${ChromiumRoot}/base/synchronization/lock.cc
${ChromiumRoot}/base/synchronization/lock_impl_posix.cc
${ChromiumRoot}/base/strings/utf_string_conversions.cc
${ChromiumRoot}/base/strings/utf_string_conversion_utils.cc
${ChromiumRoot}/base/strings/string_util.cc
${ChromiumRoot}/base/strings/string_piece.cc
${ChromiumRoot}/base/strings/string16.cc
${ChromiumRoot}/base/debug/alias.cc
${ChromiumRoot}/base/debug/debugger.cc
)


add_executable(unittest.bin
    ${ChromiumTesting}/gmock/src/gmock-all.cc
    ${ChromiumTesting}/gtest/fused/gtest/gtest-all.cc
    ${set0}
    ${chromiumTests}
)


