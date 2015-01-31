project(time-triggered-arch)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(./src SRC_LIST)

set(GMock "../../3rdparty/gmock-1.6.0/fused-src")
set(Canary "./src/canary/")

# Test framework
include_directories(${GMock}/)
include_directories(./src/)

file( GLOB  canarySchedulerRaw  ${Canary}/scheduler_raw/*.c ${Canary}/scheduler_raw/*.h)
file( GLOB  canaryScheduler  ${Canary}/scheduler/*.c)
file( GLOB  canaryLogicalDevices  ${Canary}/logical_devices/*.c)
file( GLOB  canaryVM  ${Canary}/vm/*.c)
file( GLOB  canaryTasks  ${Canary}/tasks/*.c)

file( GLOB test ./test/*.cc)

add_executable(
    ${PROJECT_NAME}
    ${SRC_LIST}
    ${canarySchedulerRaw}
    ${canaryScheduler}
    ${canaryLogicalDevices}
    ${canaryVM}
    ${canaryTasks}
    ${test}
    ${GMock}/gmock-gtest-all.cc)

