
cmake_minimum_required(VERSION 3.13)

project(
    RomuRandomCPP
    VERSION 0.0.1
    DESCRIPTION "Header Only C++ Library For Working with RomuRandom RNG"
    LANGUAGES CXX
)



add_library(${PROJECT_NAME} src/RomuRandom.cpp src/RomuRandom.hpp)

