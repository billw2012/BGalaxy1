add_library(Utils STATIC WaitHandle.cpp)

# Expose MyLibs/ root so consumers can #include "Utils/HRTimer.h" etc.
target_include_directories(Utils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)
