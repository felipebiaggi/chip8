cmake_minimum_required(VERSION 3.15)
project(chip8)

set(CMAKE_CXX_STANDARD 14)

add_executable(chip8 main.cpp Chip8.cpp Chip8.h Memoria.cpp Memoria.h)