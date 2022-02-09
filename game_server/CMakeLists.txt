# CMakeList.txt: game_server 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable (game_server "game_server.cpp")

add_definitions(-DASIO_STANDALONE)

set(INCLUDE_PATHS
   ${CMAKE_SOURCE_DIR}/mtl
   ${CMAKE_SOURCE_DIR}/common
   ${CMAKE_SOURCE_DIR}/net
   ${CMAKE_SOURCE_DIR}/deps/
)

set(LIBRARIES  
    common
    net 
)

set(LIBS_PATH
    ${CMAKE_SOURCE_DIR}/libs/
)

#link_directories(${LIBS_PATH})

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    list(APPEND LIBRARIES -pthread)
ENDIF()

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set_target_properties(game_server PROPERTIES LINK_FLAGS /INCLUDE:__tcmalloc)

    #[[add_library(libtcmalloc_minimal SHARED IMPORTED )
    set_target_properties(libtcmalloc_minimal PROPERTIES IMPORTED_LOCATION "${LIBS_PATH}/libtcmalloc_minimal.dll")
    list(APPEND LIBRARIES libtcmalloc_minimal)]]
ENDIF()

list(APPEND LIBRARIES ${LIBS_PATH}libtcmalloc_minimal${CMAKE_STATIC_LIBRARY_SUFFIX})

message(1111${LIBRARIES})

target_include_directories(game_server PUBLIC 
                           "${INCLUDE_PATHS}" 
                           ) 

target_link_libraries(game_server PUBLIC ${LIBRARIES}) 

