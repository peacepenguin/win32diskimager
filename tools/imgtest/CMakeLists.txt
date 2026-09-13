# Standalone build of the image-decoder harness. Self-contained: point cmake at
# this directory and it finds the sources next door.
#
#   cmake -S tools/imgtest -B build-imgtest -G Ninja
#   cmake --build build-imgtest
#   ./build-imgtest/imgtest.exe
#
# Not part of the application build. imagesource.cpp is compiled straight into
# the harness so the code under test is the shipped code, not a copy.
cmake_minimum_required(VERSION 3.21)
project(imgtest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APPSRC "${CMAKE_CURRENT_SOURCE_DIR}/../../src" CACHE PATH "application sources")

# Core only: ImageSource needs no widgets, and the fixtures are built with the
# same zlib and liblzma the application decodes with.
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)

add_executable(imgtest
    main.cpp
    ${APPSRC}/imagesource.cpp
)

target_include_directories(imgtest PRIVATE ${APPSRC})
target_compile_definitions(imgtest PRIVATE
    WINVER=0x0601
    _WIN32_WINNT=0x0601
)
target_link_libraries(imgtest PRIVATE Qt6::Core ZLIB::ZLIB LibLZMA::LibLZMA)

# imagesource.cpp uses the ANSI Win32 API; Qt6 turns UNICODE on by default.
qt6_disable_unicode_defines(imgtest)
