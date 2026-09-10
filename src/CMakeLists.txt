cmake_minimum_required(VERSION 3.21)
project(Win32DiskImager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

set(LANGUAGES es it pl nl de fr zh_CN zh_TW ta_IN ko ja)

# translations.qrc references lang/*.qm relative to this dir, so build them in-tree.
set(QM_FILES "")
foreach(lang ${LANGUAGES})
    set(ts "${CMAKE_CURRENT_SOURCE_DIR}/lang/diskimager_${lang}.ts")
    set(qm "${CMAKE_CURRENT_SOURCE_DIR}/lang/diskimager_${lang}.qm")
    add_custom_command(OUTPUT ${qm}
        COMMAND Qt6::lrelease ${ts} -qm ${qm}
        DEPENDS ${ts} VERBATIM)
    list(APPEND QM_FILES ${qm})
endforeach()
add_custom_target(translations DEPENDS ${QM_FILES})

qt_add_executable(Win32DiskImager WIN32
    disk.cpp
    main.cpp
    mainwindow.cpp
    droppablelineedit.cpp
    elapsedtimer.cpp
    mainwindow.ui
    gui_icons.qrc
    translations.qrc
    DiskImager.rc
)
add_dependencies(Win32DiskImager translations)

target_compile_definitions(Win32DiskImager PRIVATE
    VER="${PROJECT_VERSION}"
    WINVER=0x0601
    _WIN32_WINNT=0x0601
)

target_link_libraries(Win32DiskImager PRIVATE Qt6::Widgets)

# This code uses the ANSI Win32 API; Qt6 turns UNICODE on by default.
qt6_disable_unicode_defines(Win32DiskImager)

set_target_properties(Win32DiskImager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
