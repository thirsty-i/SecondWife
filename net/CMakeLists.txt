add_library(net STATIC 
 
"endpoint.cpp"
"socket_ops.cpp"
"ip/address_v4.cpp" 
"ip/address_v6.cpp" 
"ip/address.cpp"
"epoll/epoll.cpp"
"iocp/iocp.cpp" 
 "package_allocator.hpp"
  "descriptor_data.h"  "epoll/session.cpp"  "events/event_base.h" "msg_header.h")

# includes
include_directories(
   ${CMAKE_SOURCE_DIR}/mtl
   ${CMAKE_SOURCE_DIR}/common
   ${CMAKE_SOURCE_DIR}/net
   ${CMAKE_SOURCE_DIR}/deps/jemalloc/include
)