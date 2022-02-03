add_library(net STATIC 
 
"endpoint.cpp"
"socket_ops.cpp"
"ip/address_v4.cpp" 
"ip/address_v6.cpp" 
"ip/address.cpp"
"epoll/epoll.cpp"
"iocp/iocp.cpp" 
 "epoll/socket.h"  "ip/tcp.h" )

