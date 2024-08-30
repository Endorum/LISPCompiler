cmake_minimum_required(VERSION 3.28)
project(V1_0)

set(CMAKE_CXX_STANDARD 17)

add_executable(V1_0 src/main.cpp
        include/Lexer.hpp
        include/Parser.hpp
        include/Evaluator.hpp)
