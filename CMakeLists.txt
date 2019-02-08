cmake_minimum_required(VERSION 3.12)
project(awesome_viewer)

set(CMAKE_CXX_STANDARD 14)

set(VIEWER_SOURCE
        src/utils.hpp
        src/Pixel.hpp
        src/Cell.hpp
        src/VirtualTerminal.hpp
        src/Style.hpp
        src/StyleString.hpp)
add_library(AwesomeViewer STATIC ${VIEWER_SOURCE})
set_target_properties(AwesomeViewer PROPERTIES LINKER_LANGUAGE CXX)

add_executable(AwesomeViewerExample src/main.cpp)
target_link_libraries(AwesomeViewerExample AwesomeViewer)
