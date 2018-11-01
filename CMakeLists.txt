cmake_minimum_required(VERSION 3.12)
project(awesome_viewer)

set(CMAKE_CXX_STANDARD 14)

set(VIEWER_SOURCE
        utils.hpp
        Pixel.hpp
        Cell.hpp
        VirtualTerminal.hpp
        Style.hpp)
add_library(AwesomeViewer STATIC ${VIEWER_SOURCE})
set_target_properties(AwesomeViewer PROPERTIES LINKER_LANGUAGE CXX)

add_executable(main main.cpp)
target_link_libraries(main AwesomeViewer)