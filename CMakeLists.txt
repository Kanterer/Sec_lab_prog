cmake_minimum_required(VERSION 3.20)

project(sec_lab_prog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(lab2
        src/main.cpp
        src/tests.cpp
        src/ui.cpp
)
