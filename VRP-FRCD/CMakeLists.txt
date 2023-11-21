cmake_minimum_required(VERSION 3.10)
project(CI) 
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

set(PROJECT_DIR ${PROJECT_SOURCE_DIR}/)
set (INPUT ${PROJECT_DIR}input/)
set (OUTPUT ${PROJECT_DIR}output/)

configure_file(${PROJECT_DIR}/include/config.h.in ${PROJECT_DIR}/include/config.hpp)

file(GLOB SOURCES src/*.cpp include/*.hpp) 

add_library(CI SHARED ${SOURCES})

target_include_directories(CI PUBLIC ${PROJECT_DIR}/include)

add_compile_options(-std=c++20 -Ofast -ltbb -fPIC -march=native -Wall -Wpedantic -Wextra)

add_executable(main src/main.cpp)

target_link_libraries(main CI)