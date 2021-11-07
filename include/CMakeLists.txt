file(GLOB_RECURSE FILES *.h *.hpp)
add_library(include STATIC ${FILES} test.cpp)
