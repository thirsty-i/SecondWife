# CMakeList.txt: game_server 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable (game_server "game_server.cpp")

target_link_libraries(game_server -pthread common net)


# includes
include_directories(
   ${CMAKE_SOURCE_DIR}/mtl
   ${CMAKE_SOURCE_DIR}/common
   ${CMAKE_SOURCE_DIR}/net
)

# includes
#[[include_directories(
    ${CMAKE_SOURCE_DIR}/deps/jemalloc-5.2.0/include
)]]

#link_directories(${CMAKE_SOURCE_DIR}/libs/libjemalloc.a)



#[[target_link_libraries(game_server
    -pthread
    ${CMAKE_SOURCE_DIR}/libs/libjemalloc.a
    -ldl
)
#target_link_libraries(game_server jemalloc)
]]

