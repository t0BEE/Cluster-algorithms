cmake_minimum_required(VERSION 3.7)
project(BIRCH)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(SOURCE_FILES BIRCH.cpp CFTreeNode.cpp ClusteringFeature.cpp Parameter.cpp Point.cpp Cluster.cpp Cluster.h)
add_executable(BIRCH ${SOURCE_FILES})