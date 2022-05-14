cmake_minimum_required(VERSION 3.0.0)
project(dr VERSION 0.1.0)

include(CTest)
include_directories(FFVIII_DR)
add_library(dr SHARED FFVIII_DR/dllmain.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include_directories(FFVIII_DR_testZone)
add_executable(testZone FFVIII_DR_testZone/main.cpp)