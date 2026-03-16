add_library(Ugly STATIC
    ProcessQueue.cpp
    ProcessRedirect.cpp
    QDebugStream.cpp
    UglyGraphicsView.cpp
    ugly.cpp
    # Headers with Q_OBJECT must be listed so AUTOMOC picks them up
    ProcessQueue.h
    QDebugStream.h
    UglyGraphicsView.h
)

set_target_properties(Ugly PROPERTIES AUTOMOC ON)

target_include_directories(Ugly
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

target_compile_definitions(Ugly
    PUBLIC
        UNICODE
        QT_LARGEFILE_SUPPORT
        QT_CORE_LIB
        QT_GUI_LIB
        QT_WIDGETS_LIB
        UGLY_LIB
)

target_link_libraries(Ugly
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Boost::headers
)
