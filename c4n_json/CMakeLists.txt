cmake_minimum_required(VERSION 3.25)

project(c4n_json)

set(CMAKE_CXX_STANDARD 17)

add_executable(c4n_json main.cpp entity/Json.cpp parser/Parser.cpp)
