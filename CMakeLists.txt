cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED PROJECT_NAME)
    set(NOT_SUBPROJECT ON)
endif()

project(awesome_viewer LANGUAGES CXX VERSION 0.1)

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

if(NOT_SUBPROJECT)
    add_executable(AwesomeViewerExample src/main.cpp)
    target_link_libraries(AwesomeViewerExample AwesomeViewer)
endif()
