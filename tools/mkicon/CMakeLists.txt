# Builds mkicon, which renders an SVG into a multi-size Windows .ico.
#
#   mkicon <in.svg> <out.ico>
#
# The application's cmake configures and builds this itself, separately from the
# rest, because it has to run on the machine doing the build rather than the
# Windows machine being built for. It is not meant to be invoked by hand.
# See BUILD.md.
cmake_minimum_required(VERSION 3.21)
project(mkicon LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Gui Svg)
add_executable(mkicon main.cpp)
target_link_libraries(mkicon PRIVATE Qt6::Gui Qt6::Svg)
