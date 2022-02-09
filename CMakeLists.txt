# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("SecondWife")

set(CXX_FLAGS
 -std=c++11
)

set(DEPS_DIR 
${CMAKE_SOURCE_DIR}/deps
${CMAKE_SOURCE_DIR}/deps/asio
)


# 包含子项目。
add_subdirectory("common")
add_subdirectory("net")
add_subdirectory ("game_server")