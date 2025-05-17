cmake_minimum_required(VERSION 3.28)
project(LISPCompiler)

set(CMAKE_CXX_STANDARD 20)


add_executable(LISPCompiler src/main.cpp
        src/parser.cpp
        src/tokenizer.cpp
        include/codegen.hpp
        src/preproc.cpp
        include/preproc.hpp
        src/codegen.cpp)
        # include/asmgen.hpp
        # src/asmgen.cpp
        # src/asmgen.cpp
        # src/asmgen.cpp
        # include/asmgen.hpp)
