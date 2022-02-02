add_library(net STATIC 
"net.cpp" 
"epoll/epoll.cpp" 
)

target_link_libraries(net "common")
