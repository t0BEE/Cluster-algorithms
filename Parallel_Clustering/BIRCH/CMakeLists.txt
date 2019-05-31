cmake_minimum_required(VERSION 3.13.4)
project(BIRCH)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

include_directories(.)

add_executable(BIRCH
        BIRCH.cpp
        BIRCH.h
        CFTreeNode.cpp
        CFTreeNode.h
        ClusteringFeature.cpp
        ClusteringFeature.h
        Parameter.cpp
        Parameter.h
        Point.cpp
        Point.h)
