aux_source_directory(. SRCS)
add_library(net STATIC ${SRCS} "buffer.hpp"  "net.cpp")

include_directories( 
    ${DEPS_DIR}
    ${PROJECT_SOURCE_DIR}
) 

