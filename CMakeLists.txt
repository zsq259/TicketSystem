cmake_minimum_required(VERSION 3.10.2)
project(bookstore)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -Ofast")

add_executable(code ${src_dir} src/main.cpp src/parser.cpp src/user.cpp src/train.cpp src/ticket.cpp)
#add_executable(code ${src_dir} b+tree/main.cpp)