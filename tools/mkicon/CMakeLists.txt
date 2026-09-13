# Builds mkicon, which renders an SVG into a multi-size Windows .ico.
#
#   cmake -S tools/mkicon -B build-mkicon -G Ninja
#   cmake --build build-mkicon
#   ./build-mkicon/mkicon.exe src/images/Win32DiskImager.svg src/images/Win32DiskImager.ico
#
# Not part of the application build: the .ico it produces is committed, and this
# only has to run when the icon changes. See BUILD.md.
cmake_minimum_required(VERSION 3.21)
project(mkicon LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Gui Svg)
add_executable(mkicon main.cpp)
target_link_libraries(mkicon PRIVATE Qt6::Gui Qt6::Svg)
