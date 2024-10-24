cmake_minimum_required(VERSION 3.15)
project(CrestTest C)

set(CMAKE_C_STANDARD 17)

add_executable(CrestTest test_main.c)

find_package(Crest CONFIG REQUIRED)
target_link_libraries(CrestTest crest)
