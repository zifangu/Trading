cmake_minimum_required(VERSION 3.15)
project(project_3 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_executable(project_3 besttrade.c besttrade.h main.c)