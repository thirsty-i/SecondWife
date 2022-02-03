# CMakeList.txt: game_server 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# includes
include_directories(
    ${CMAKE_SOURCE_DIR}/mtl
    ${CMAKE_SOURCE_DIR}/net
    ${CMAKE_SOURCE_DIR}/common
)

# 将源代码添加到此项目的可执行文件。
add_executable (game_server "game_server.cpp")


target_link_libraries(game_server common net)
# TODO: 如有需要，请添加测试并安装目标。
