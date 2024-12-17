cmake_minimum_required(VERSION 3.27)
project(projet_exe C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(projet_exe main.c src/fonction.c)
