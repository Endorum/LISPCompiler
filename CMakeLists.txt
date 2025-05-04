cmake_minimum_required(VERSION 3.28)
project(LISPCompiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(LISPCompiler src/main.cpp
        src/parser.cpp
        src/tokenizer.cpp
        include/codegen.hpp
        src/preproc.cpp
        include/preproc.hpp)
