cmake_minimum_required(VERSION 3.10)
project(tortoise-javah)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BUILD_SHARED_LIBS OFF)

set(SOURCES
    main.cpp
)

add_executable(tortoise-javah ${SOURCES})