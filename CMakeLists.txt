cmake_minimum_required(VERSION 3.10)
project(tortoise-javah)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL /MT /GS /EHsc")

set(BUILD_SHARED_LIBS OFF)

set(SOURCES
    main.cpp
    src/Function.cpp
    src/Header.cpp
    src/TortoiseJAVAH.cpp
)

set(HEADERS
    include/Function.hpp
    include/Header.hpp
    include/Constants.hpp
    include/TortoiseJAVAH.hpp
)

add_executable(tortoise-javah ${SOURCES} ${HEADERS})