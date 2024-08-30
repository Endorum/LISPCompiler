cmake_minimum_required(VERSION 3.28)
project(V3_0)

set(CMAKE_CXX_STANDARD 17)

# Include SDL2 and SDL2_image headers
include_directories("/usr/local/Cellar/sdl2/2.30.6/include")
include_directories("/usr/local/Cellar/sdl2_image/2.8.2_1/include/SDL2")

# Add the executable target
add_executable(V3_0 src/main.cpp include/testclass.hpp)

# Find SDL2 and SDL2_image packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Link the SDL2 and SDL2_image libraries
target_link_libraries(V3_0 PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)
