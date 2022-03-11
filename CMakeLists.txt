cmake_minimum_required(VERSION 3.21)
project(osu_sr_calculator)

set(CMAKE_CXX_STANDARD 14)
# DEBUG:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address,undefined,integer")

include_directories(osu_sr_calculator)

add_executable(osu_sr_calculator
        osu_sr_calculator/main.cpp
        osu_sr_calculator/BeatmapParser.cpp
        osu_sr_calculator/DifficultyHitObjectCreator.cpp
        osu_sr_calculator/StarRatingCalculator.cpp)
