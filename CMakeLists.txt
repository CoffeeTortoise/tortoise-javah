cmake_minimum_required(VERSION 3.10)
project(tortoise-javah)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BUILD_SHARED_LIBS OFF)

set(SOURCES
    main.cpp
    src/Function.cpp
    src/Header.cpp
)

set(HEADERS
    include/Function.hpp
    include/Header.hpp
    include/Constants.hpp
)

add_executable(tortoise-javah ${SOURCES} ${HEADERS})