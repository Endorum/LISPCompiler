cmake_minimum_required(VERSION 3.28)
project(FactoryGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(FactoryGame main.cpp
        Game.hpp
        World.hpp
        Tile.hpp
        Entity.hpp
        Machine.hpp
        Resource.hpp
        Inventory.hpp
        Game.cpp
)
