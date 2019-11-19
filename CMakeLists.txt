cmake_minimum_required(VERSION 3.14)
project(cmpeshell)

set(CMAKE_CXX_STANDARD 14)

add_executable(cmpeshell main.cpp command.cpp command.h pipe_options.h)