cmake_minimum_required(VERSION 3.27)
project(Labb1_dt096g)

set(CMAKE_CXX_STANDARD 17)

add_executable(Labb1_dt096g main.cpp
        lexer.h
        parser.cpp
        parser.h)
