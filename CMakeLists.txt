# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("SecondWife")


# includes
include_directories(
   ${CMAKE_SOURCE_DIR}/mtl
   ${CMAKE_SOURCE_DIR}/common
   ${CMAKE_SOURCE_DIR}/net
   ${CMAKE_SOURCE_DIR}/deps
)

set(CXX_FLAGS
 -std=c++11
)

add_definitions(-DASIO_STANDALONE)

# 包含子项目。
add_subdirectory ("game_server")
add_subdirectory("common")
add_subdirectory("net")


# specify the C++ standard 
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")



