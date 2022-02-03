add_library(net STATIC 
"net.cpp" 
"epoll/epoll.cpp" 
 "iocp/iocp.h" "iocp/iocp.cpp" "epoll/accept.h" "endpoint.cpp" "endpoint.h" "address.cpp" "address.h" "address_v4.h" "address_v6.h" "address_v4.cpp" "address_v6.cpp")

target_link_libraries(net "common")
