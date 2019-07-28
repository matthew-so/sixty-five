cmake_minimum_required(VERSION 3.12)
project(Suteru C)

set(CMAKE_C_STANDARD 11)

add_executable(Suteru main.c mem.c cpu.c mem.h)