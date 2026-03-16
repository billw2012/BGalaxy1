# Header-only library — no compiled sources
add_library(Math INTERFACE)

target_include_directories(Math
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

# Math/highprec.h uses hpalib
target_link_libraries(Math INTERFACE hpalib)
