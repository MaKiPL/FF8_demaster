cmake_minimum_required(VERSION 3.18)
project(ff8_demaster LANGUAGES C CXX ASM_MASM)
# CSharp is supported but only via Visual Studio, not via Nmake.

add_subdirectory(ff8_demaster)
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    # Do Visual Studio specific stuff
    add_subdirectory(viiidem_customlauncher)
endif ()