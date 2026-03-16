set(SOURCES
    BHTree.cpp
    BHTree.h
    GalaxyGizmo.cpp
    GalaxyGizmo.h
    GalaxySerializer.cpp
    GalaxySerializer.h
    GalaxySim.cpp
    GalaxySim.h
    GalaxySimGLWidget.h   # listed so AUTOMOC sees Q_OBJECT
    GalaxyTypes.h
    OpenCLWrappers.cpp
    OpenCLWrappers.h
    galaxy1.cpp
    galaxy1.h             # listed so AUTOMOC sees Q_OBJECT
    galaxy_c.h
    main.cpp
)

# Track CL kernel files so edits trigger a rebuild (and therefore CopyKernels)
file(GLOB CL_KERNEL_FILES "${CMAKE_SOURCE_DIR}/Galaxy1/Data/CL/*.cl")
set_source_files_properties(${CL_KERNEL_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

set(UI_FILES
    galaxy1.ui
    GalaxyWidget.ui
)

set(QRC_FILES
    galaxy1.qrc
)

add_executable(Galaxy1
    ${SOURCES}
    ${UI_FILES}
    ${QRC_FILES}
    ${CL_KERNEL_FILES}
)

set_target_properties(Galaxy1 PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    # Look for generated UI headers in the build dir
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_definitions(Galaxy1 PRIVATE
    UNICODE
    WIN32
    QT_LARGEFILE_SUPPORT
    QT_CORE_LIB
    QT_GUI_LIB
    QT_WIDGETS_LIB
    QT_OPENGL_LIB
    # Uncomment to enable double-precision OpenCL simulation:
    # GALAXY_DOUBLE_PRECISION
)

target_link_libraries(Galaxy1 PRIVATE
    Ugly
    Utils
    Math
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
    OpenCL::OpenCL
    Boost::thread
    Boost::random
    dlib::dlib
    ${OpenCV_LIBS}
    opengl32
    glu32
)

# Copy OpenCL kernel files — always runs (custom targets are always out-of-date,
# so this fires on every build/F5 even when no C++ sources changed)
add_custom_target(CopyKernels ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Galaxy1/Data"
        "$<TARGET_FILE_DIR:Galaxy1>/Data"
    COMMENT "Copying Data directory (CL kernels) to output"
)
add_dependencies(Galaxy1 CopyKernels)

# Deploy Qt DLLs and platform plugins next to the executable
find_program(WINDEPLOYQT windeployqt HINTS "${Qt5_DIR}/../../../bin")
if(WINDEPLOYQT)
    add_custom_command(TARGET Galaxy1 POST_BUILD
        COMMAND ${WINDEPLOYQT}
            $<$<CONFIG:Debug>:--debug>
            $<$<CONFIG:Release>:--release>
            --no-compiler-runtime
            --no-translations
            "$<TARGET_FILE:Galaxy1>"
        COMMENT "Running windeployqt"
    )
endif()
