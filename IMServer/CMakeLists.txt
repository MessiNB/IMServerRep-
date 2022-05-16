cmake_minimum_required (VERSION 3.8) #需要的最小版本

project ("IMServer") #项目名

# 设置 编译参数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -O0  -Wno-unused-variable -pthread")

add_subdirectory(src bin)