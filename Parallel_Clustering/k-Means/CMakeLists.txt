cmake_minimum_required(VERSION 3.7)
project(k_Means)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(SOURCE_FILES kMeans.cpp kParameter.h kPoint.cpp kCluster.cpp)
add_executable(k_Means ${SOURCE_FILES})