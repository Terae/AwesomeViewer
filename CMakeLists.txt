cmake_minimum_required(VERSION 3.12)
project(awesome_viewer)

set(CMAKE_CXX_STANDARD 14)

set(VIEWER_SOURCE
        Cell.cpp
        VirtualTerminal.cpp
        Style.hpp
        Pixel.hpp)
add_library(AwesomeViewer STATIC ${VIEWER_SOURCE})

add_executable(main main.cpp)
target_link_libraries(main AwesomeViewer)