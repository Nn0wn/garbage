cmake_minimum_required(VERSION 2.8)
project(lab_5)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

find_package(GTest REQUIRED)
include_directories(include ${GTEST_INCLUDE_DIRS})

set (SOURCE src/lab_6.cpp src/bohr.cpp)
set (JOKSOURCE src/joker.cpp src/bohr.cpp)

if (main)
    add_executable(lab_6 ${SOURCE})
endif()
if (joker)
    add_executable(lab_6_jok ${JOKSOURCE})
endif()
