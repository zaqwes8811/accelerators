# debig troubles
# http://stackoverflow.com/questions/19912648/cmake-qtcreator-mingw-debug-mode-windows-7
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# -DCMAKE_BUILD_TYPE=Debug
# -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#
#
# -DCMAKE_TOOLCHAIN_FILE=/home/zaqwes/work/algorithmic-space/platforms/llvm-clang-cilkplus.toolchain.cmake

project(alg-space)
cmake_minimum_required(VERSION 2.8)

#Message(${CMAKE_CURRENT_LIST_DIR})

set(Extern "./extern/")
set(GMock "./extern/gmock-1.6.0/fused-src")
set(Canary "./source/canary/")
set(Pro "./source/production/")
set(unitTests "./tests/units/")
set(Source "source/")
set(testsRoot "./tests")
set(root ".")

#set(BigExtern "D:/work/big-3rdparty/")
set(BigExtern "/home/zaqwes/big-3rdparty/")

set(Intern "./intern/")
set(User "/home/zaqwes")

# Test framework
include_directories(${GMock})
include_directories(${Source})
include_directories(${Source}/reuse)
include_directories(${Extern}/slre-1.3)
include_directories(${Extern}/loki-0.1.7/include)
include_directories(${Extern})
include_directories(${Extern}/mongoose-5.2/)
include_directories(${Extern}/smash-vx.y-fix/)
include_directories(${Extern}/jsoncpp-src-0.6.0-rc2/include)
include_directories(${BigExtern}/boost_1_57_0)
include_directories(${BigExtern}/asl_1.0.43)
include_directories(${unitTests})
include_directories(${User}/opt/clang-cilkplus/include)
include_directories(/home/zaqwes/vdisks/tools/clang-cilkplus/archive-linux/lib)

set(GMock "./extern/gmock-1.6.0/fused-src")
set(Canary "./source/time_triggered_arch")

# Test framework
include_directories(${GMock}/)
include_directories(./src/)

file( GLOB  canarySchedulerRaw  ${Canary}/scheduler_raw/*.c ${Canary}/scheduler_raw/*.h)
file( GLOB  canaryScheduler  ${Canary}/scheduler/*.c)
file( GLOB  canaryLogicalDevices  ${Canary}/logical_devices/*.c)
file( GLOB  canaryVM  ${Canary}/vm/*.c)
file( GLOB  canaryTasks  ${Canary}/tasks/*.c)

# link the libraries to the executable
if (MINGW)
  file( GLOB toRemoveSnmpAgent ${Pro}/win_snmp_update/*.cc ${Pro}/win_snmp_update/*.h)
  # Mingw c++11 troubles
  # https://www.ics.uci.edu/~pattis/common/modules46/googletest.html
  # http://stackoverflow.com/questions/13198712/trouble-building-tests-with-google-test-clang-and-libc
  # https://code.google.com/p/googletest/issues/detail?id=422
  # -stdlib=libc++  - not work
  # -std=gnu++11
  # http://stackoverflow.com/questions/15053776/how-do-you-disable-the-unused-variable-warnings-coming-out-of-gcc
  # -Wno-unused-variable
  link_directories(${BigExtern}/boost_1_57_0/stage/lib)
  add_definitions(  -Wall -Wno-unused-local-typedefs)# -Wincompatible-pointer-types)
  #add_definitions(-DGTEST_USE_OWN_TR1_TUPLE=1 -DGTEST_HAS_TR1_TUPLE=1)
  #LINK_LIBRARIES(ws2_32 WSnmp32 boost_system-mgw48-mt-s-1_57)
elseif (MSVC90)
  file( GLOB toRemoveSnmpAgent ${Pro}/win_snmp_update/*.cc ${Pro}/win_snmp_update/*.h)
  # FIXME: C runtime troubles
  # http://stackoverflow.com/questions/14172856/cmake-compile-with-mt-instead-of-md
  # http://msdn.microsoft.com/en-us/library/2kzt1wy3.aspx
  # http://stackoverflow.com/questions/757418/should-i-compile-with-md-or-mt
  #
  # http://rsdn.ru/forum/cpp.applied/2843738.hot
  # /MT
  link_directories(${BigExtern}/boost_1_57_0/stage/lib)
  add_definitions( -D_CRT_SECURE_NO_WARNINGS)# /showIncludes)#D)#T)
  #set(CMAKE_CXX_FLAGS_RELEASE "/MT")

  # Failure
  #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  #LINK_LIBRARIES(ws2_32 WSnmp32 libboost_system-vc90-mt-1_57)# libboost_thread-vc90-mt-s-1_57.lib)
elseif (UNIX)
  # gcc 4.8 ubuntu 12 http://llvm.org/docs/GettingStarted.html#getting-a-modern-host-c-toolchain
  # cmake .. -DCMAKE_CXX_LINK_FLAGS="-Wl,-rpath,$HOME/toolchains/lib64 -L$HOME/toolchains/lib64"
  # http://llvm.org/docs/GettingStarted.html#getting-a-modern-host-c-toolchain

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Static analyser http://clang-analyzer.llvm.org/
    # https://github.com/couchbase/couchnode/blob/master/deps/lcb/cmake/Modules/GetLibcouchbaseFlags.cmake
    # http://blogs.perl.org/users/rurban/2011/11/adventures-with-clang-and-asan.html
    #IF(LCB_USE_ASAN)
    # main: address - thread, memory and ub not work in i686, only x86_64
    #-fno-omit-frame-pointer -O1
    #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g  -fsanitize=address")
    #-fno-omit-frame-pointer -O1
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g  -fsanitize=address")
    # -undefined dynamic_lookup
    #    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fsanitize=address")
    #    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    #ENDIF()
    # TOOLS:
    #
    # https://code.google.com/p/address-sanitizer/wiki/HowToBuild
    # http://cameo54321.blogspot.ru/2014/02/installing-cmake-288-or-higher-on.html
    # clang 3.1 >= need for ASan Ub12.04 -> 3.0
    #
    # !!https://solarianprogrammer.com/2013/01/17/building-clang-libcpp-ubuntu-linux/
    # Try it! https://github.com/pacs-course/pacs/wiki/Instructions-to-install-clang-3.1-on-ubuntu-12.04.1-and-12.10
    # http://stackoverflow.com/questions/17657261/how-to-install-clang-pre-built-binaries-ubuntu-12-04
    # http://sohanikh.blogspot.ru/2013/02/how-to-build-llvm-from-source-on-ubuntu.html
    #add_definitions(  -Wall -g -fsanitize=address)
    # https://developer.mozilla.org/en-US/docs/Mozilla/Testing/Firefox_and_Address_Sanitizer
    message("I'm clang")
    link_directories(${BigExtern}/boost_1_57_0/stage_gcc/lib)
    link_libraries(tbb tbbmalloc pthread rt)  # !! pthread and rt last!!
  else()
    message("I'm NOT clang")
    add_definitions(  -Wall )#-std=c++11)
    # FIXME: -D_REENTRANT
    if (CMAKE_SYSTEM_PROCESSOR MATCHES arm)
      link_directories(${BigExtern}/boost_1_57_0/stage_arm/lib)
      #link_libraries(pthread boost_system)
      message("I'm ARM processor")
      # http://stackoverflow.com/questions/11944060/how-to-detect-target-architecture-using-cmake
      #  message( STATUS "Architecture: ${ARCHITECTURE}" ) # not work
    else()
      IF(LCB_USE_VALGRIND)
          # -pg
          #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
          #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
          #SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup -fsanitize=address")
          #SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
      ENDIF()

      message("I'm NOT ARM processor")
      add_definitions(  -std=c++0x)
      link_directories(${BigExtern}/boost_1_57_0/stage_gcc/lib)
      # how split embedded form just linux
      # http://stackoverflow.com/questions/9534126/automatically-linking-socket-shared-library-in-nix
      # http://stackoverflow.com/questions/14835605/linking-with-gcc-fails-after-switching-boost-version-from-1-52-to-1-53
      #
      # http://stackoverflow.com/questions/9511422/undefined-reference-to-clock-gettime-gcc-4-6-cmake
      # http://www.unix.com/man-page/opensolaris/3lib/librt/
      #
      # https://software.intel.com/en-us/forums/topic/285308
      link_libraries(tbb tbbmalloc boost_system boost_thread  pthread rt)  # !! pthread and rt last!!
    endif()
  endif()
endif()

message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

# Sources
file( GLOB canaryHttp
  ${Source}/details/*.cc
  ${Source}/async-parallel/*.h
  ${Source}/async-parallel/*.cc
  ${Source}/canary/*.cc
  ${Source}/heart/*.h
  ${Source}/canary/*.h)

file( GLOB proInMemoryStorage ${Pro}/sampler/*.cc)
file( GLOB proCross ${Pro}/crosscuttings/*.cc)
file( GLOB proCommon ${Pro}/crosscuttings/Common/*.cc)
file( GLOB proJournals ${Pro}/journals/*.cc)
file( GLOB proInnerReuse
  ${Pro}/inner_reuse/*.cc
  ${Pro}/inner_reuse/*.h
  ${Pro}/inner_reuse/*.c
)

file( GLOB proFSMs
    ${Pro}/fsm/*.cc)

file( GLOB proSNMPTop
    ${Pro}/snmp/*.h
    ${Pro}/snmp/*.cc)

file( GLOB proWebAccess
    ${Pro}/web_access/*.cc)

file( GLOB proCAgent
    ${Pro}/pure_c_snmp/*.c
    ${Pro}/pure_c_snmp/*.cc
    ${Pro}/pure_c_snmp/*.h)

file( GLOB proGlue  ${Pro}/glue/*.h)

file( GLOB apple
  ${Extern}/hypertable-fix/Common/*.cc
  ${Extern}/hypertable-fix/*.cc)

file( GLOB slre  ${Extern}/slre-1.3/*.c)
file( GLOB mongoose  ${Extern}/mongoose-5.2/*.c)
file( GLOB jsonCpp  ${Extern}/jsoncpp-src-0.6.0-rc2/src/lib_json/*.cpp)
file( GLOB snmp
    ${Extern}/smash-vx.y-fix/smash/*.c
    ${Extern}/smash-vx.y-fix/smash/*.cc)
file( GLOB asn1Lib
    ${Extern}/smash-vx.y-fix/asn1/*.c)
file( GLOB externMibApi
    ${Extern}/smash-vx.y-fix/mib_api/*.c
    ${Extern}/smash-vx.y-fix/mib_api/*.cc
    ${Extern}/smash-vx.y-fix/mib_api/*.h)

# unittests
file( GLOB mibTest  ${unitTests}/canary/smash_test_mib/*.c)
file( GLOB canaryTestHttp  ${unitTests}/canary/*.cc ${Tests}/canary/*.h)
file( GLOB proTests
    ${unitTests}/tricks_peoples/sean_parent/*.cc
    ${unitTests}/tbb/*.cc
    ${unitTests}/*.cc
    ${unitTests}/train/*.cc
    ${unitTests}/stanford_algoritms_part2/*.cc
    ${unitTests}/tricks_intermediate/*.cc
    ${unitTests}/async-parallel/*.cc
    ${unitTests}/mem_heap_stack/*.cc
    ${unitTests}/extern/*.cc
    ${unitTests}/async-parallel/patterns/*.cc
    ${unitTests}/stl/*.cc
    ${unitTests}/tbb/*.cc
    ${unitTests}/async-parallel-computing/safe_data_structures/*.cc
    ${unitTests}/async-parallel-computing/boost_cc98/*.cc)

file( GLOB testsRtos
    ${unitTests}/async-parallel-computing/*.cc)

# Pack
set( unitTests ${mibTest} ${canaryTestHttp} ${proTests} ${testsRtos})
set( externCode ${apple} ${slre} ${mongoose} ${jsonCpp} ${snmp} ${asn1Lib} ${externMibApi})
set( proSources
    ${proGlue}
    ${proCommon}
    ${proFSMs}
    ${proSNMPTop}
    ${proInnerReuse}
    ${proCAgent}
    ${proWebAccess}
    ${proCross}
    ${proInMemoryStorage}
    ${proJournals})

set(canarySources
    ${canaryHttp})

file(GLOB blockedTestSources
    ${testsRoot}/*.cc)

# Warning: run one by one and !in QtCreator only in external terminal!
add_executable(blocked_tests.bin
    ${Source}/mains/launcher_tests.cc
    ${GMock}/gmock-gtest-all.cc
    ${externCode}
    ${blockedTestSources}
    ${proSources}
    ${canarySchedulerRaw}
    ${canaryScheduler}
    ${canaryLogicalDevices}
    ${canaryVM}
    ${canaryTasks}
    ${test}
    ${canarySources}
)

add_executable(unittest.bin
    ${Source}/mains/launcher_tests.cc
    ${GMock}/gmock-gtest-all.cc
    ${canarySchedulerRaw}
    ${canaryScheduler}
    ${canaryLogicalDevices}
    ${canaryVM}
    ${canaryTasks}
    ${test}
    ${unitTests}
    ${externCode}
    ${proSources}
    ${canarySources}
)

