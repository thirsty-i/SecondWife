# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("SecondWife")

# 包含子项目。
add_subdirectory ("game_server")

# specify the C++ standard 
#set(CMAKE_CXX_STANDARD 14) 
#set(CMAKE_CXX_STANDARD_REQUIRED ON) 
#set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_FLAGS -std=c++11)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

message("hello")
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_SOURCE_DIR})
message("hello")

# includes
target_include_directories(
	"game_server" PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mtl
	${CMAKE_CURRENT_SOURCE_DIR}/include/log
	${CMAKE_CURRENT_SOURCE_DIR}/include/common
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_definitions(-DXLOCALE_NOT_USED)