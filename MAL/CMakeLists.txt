cmake_minimum_required(VERSION 3.28)
project(MAL)

set(CMAKE_CXX_STANDARD 17)

add_executable(MAL main.cpp
        reader.cpp
        reader.hpp
        printer.cpp
        printer.hpp
        types.cpp
        types.hpp
        env.hpp
        core.cpp
        core.hpp)
