cmake_minimum_required(VERSION 3.14)
project(Telecom_Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(labyrinth 
    src/labyrinth/labyrinth.cpp
    src/labyrinth/labyrinth.hpp
    src/labyrinth/labyrinth_types.cpp
    src/labyrinth/labyrinth_types.hpp
    src/utils/types.hpp
    src/utils/config.hpp
    src/utils/calculator.hpp
)
target_include_directories(labyrinth PUBLIC src)

# 2. Библиотека бота
add_library(bot 
    src/bot/regular_bot.cpp
    src/bot/regular_bot.hpp
    src/bot/bot.hpp
)
target_include_directories(bot PUBLIC src)
target_link_libraries(bot PUBLIC labyrinth)

add_library(simulator 
    src/manager/game_manager.cpp
    src/manager/game_manager.hpp
)
target_include_directories(simulator PUBLIC src)
target_link_libraries(simulator PUBLIC labyrinth bot)

add_executable(simulator_run src/main.cpp)
target_link_libraries(simulator_run PRIVATE labyrinth bot simulator)

enable_testing()
add_subdirectory(tests)