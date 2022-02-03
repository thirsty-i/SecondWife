# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("SecondWife")

# 包含子项目。
add_subdirectory ("game_server")

# specify the C++ standard 
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CXX_FLAGS
 -std=c++11
 -pthread
)

message("hello")
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_SOURCE_DIR})
message("hello")

# includes
include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/mtl
   ${CMAKE_CURRENT_SOURCE_DIR}/common
   ${CMAKE_CURRENT_SOURCE_DIR}/net
)


add_subdirectory("common")
add_subdirectory("net")

