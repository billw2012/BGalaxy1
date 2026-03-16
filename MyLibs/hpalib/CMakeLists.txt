file(GLOB HPALIB_SOURCES src/*.c)

add_library(hpalib STATIC ${HPALIB_SOURCES})

target_include_directories(hpalib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# hpalib is C89/C90 code — suppress warnings rather than treat them as errors
target_compile_options(hpalib PRIVATE
    $<$<C_COMPILER_ID:MSVC>:/W1 /wd4996>
)
