cmake_minimum_required(VERSION 3.14)
project(Telecom_Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(labyrinth 
    src/labyrinth/labyrinth.cpp
    src/labyrinth/labyrinth.hpp
)

target_include_directories(labyrinth PUBLIC src)

enable_testing()

add_subdirectory(tests)