cmake_minimum_required(VERSION 3.20)
project(BGalaxy1 CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use dynamic CRT to match vcpkg's default x64-windows triplet
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# vcpkg toolchain sets this up — find Qt5 via QTDIR env var or CMAKE_PREFIX_PATH
if(DEFINED ENV{QTDIR})
    list(APPEND CMAKE_PREFIX_PATH $ENV{QTDIR})
endif()

# Help FindOpenCL locate the CUDA-provided OpenCL SDK
if(DEFINED ENV{CUDA_PATH} AND NOT OpenCL_INCLUDE_DIR)
    set(OpenCL_INCLUDE_DIR "$ENV{CUDA_PATH}/include" CACHE PATH "")
    set(OpenCL_LIBRARY    "$ENV{CUDA_PATH}/lib/x64/OpenCL.lib" CACHE FILEPATH "")
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)
find_package(OpenCL REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread random)
find_package(dlib REQUIRED)
find_package(OpenCV REQUIRED)

# Make external headers available to all subprojects (same pattern as Explore2)
include_directories(LegacyMyLibs)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(LegacyMyLibs/hpalib)
add_subdirectory(LegacyMyLibs/Utils)
add_subdirectory(LegacyMyLibs/Math)
add_subdirectory(LegacyMyLibs/Ugly)
add_subdirectory(Galaxy1/Galaxy1)
