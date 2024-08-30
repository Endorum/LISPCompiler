cmake_minimum_required(VERSION 3.28)
project(FactoryGame)

set(CMAKE_CXX_STANDARD 17)



add_executable(FactoryGame main.cpp
        Entity.hpp
        Tile.hpp
        machine.hpp
        Vec.hpp
        World.hpp
        Item.hpp
        Resource.hpp
        Machine.cpp
        Tile.cpp
        World.cpp
)
