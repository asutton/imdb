
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-std=c++14")
include_directories(.)

add_subdirectory(imdb)
add_subdirectory(db)
add_subdirectory(examples)

