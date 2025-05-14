# https://github.com/SFML/cmake-sfml-project
cmake_minimum_required(VERSION 3.22...4.0.1)
project(Platformer LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)

# write result to bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "-O3 -g -Wall -Werror -pedantic")

# get SFML
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# add the contents of the src folder
add_subdirectory(src)

target_compile_features(Platformer PRIVATE cxx_std_20)

# link SFML
target_link_libraries(Platformer PRIVATE SFML::Graphics)

# add assets (fonts and images) to bin folder
add_custom_target(copy_assets
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/bin/assets
)
add_dependencies(Platformer copy_assets)