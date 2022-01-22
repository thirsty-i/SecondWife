file(GLOB_RECURSE FILES *.h *.hpp)
add_library(include test.cpp ./mtl/thread/spin_lock.hpp "mtl/struct/circle_list.h")
