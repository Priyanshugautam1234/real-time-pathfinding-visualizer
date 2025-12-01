cmake_minimum_required(VERSION 3.10)

project(pathfinding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ✅ Force explicit compile and link options for MinGW
if(MINGW)
    add_compile_options(-mconsole)
    add_link_options(-mconsole)
endif()

add_executable(pathfinding
    src/main.cpp
    src/grid.cpp
    src/algorithms.cpp
)
