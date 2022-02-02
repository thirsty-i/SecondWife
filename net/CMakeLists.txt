add_library(net STATIC 
"net.cpp" 
"epoll/epoll.cpp" 
 "iocp/iocp.h" "iocp/iocp.cpp")

target_link_libraries(net "common")
