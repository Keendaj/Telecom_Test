cmake_minimum_required(VERSION 3.14)
project(Telecom_Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(labyrinth 
    src/labyrinth/labyrinth.cpp
    src/labyrinth/labyrinth.hpp
    src/utils/types.hpp
    src/utils/config.hpp
)
target_include_directories(labyrinth PUBLIC src)

add_library(bot 
    src/bot/regular_bot.cpp
    src/bot/regular_bot.hpp
    src/bot/bot.hpp
)
target_include_directories(bot PUBLIC src)

target_link_libraries(bot PUBLIC labyrinth)

enable_testing()
add_subdirectory(tests)