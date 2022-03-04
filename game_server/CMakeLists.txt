# CMakeList.txt: game_server 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable (game_server 
"game_server.cpp" 
"main.cpp" 
"connects/gate/gate_net.cpp"
"connects/gate/gate_message_handler.cpp")

set(LIBRARIES  
    common
    net 
)

set(LIBS_PATH
    ${CMAKE_SOURCE_DIR}/libs/
)

message(111${PROJECT_SOURCE_DIR})

list(APPEND LIBRARIES -pthread)
list(APPEND LIBRARIES ${LIBS_PATH}libtcmalloc_minimal${CMAKE_STATIC_LIBRARY_SUFFIX})

include_directories( 
    ${DEPS_DIR}
    ${PROJECT_SOURCE_DIR}
) 

target_link_libraries(game_server PUBLIC ${LIBRARIES}) 

