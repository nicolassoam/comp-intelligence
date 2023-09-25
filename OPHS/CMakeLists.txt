cmake_minimum_required(VERSION 3.10)
project(CI) 
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
configure_file(${PROJECT_DIR}/include/config.h.in ${PROJECT_DIR}/include/config.h)
add_compile_options(-std=c++20 -Ofast -fPIC -march=native -Wall -Wpedantic -Wextra)