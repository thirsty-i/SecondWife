aux_source_directory(. SRCS)
add_library(net STATIC ${SRCS})

include_directories( 
    ${DEPS_DIR}
    ${PROJECT_SOURCE_DIR}
) 

