#pragma once

#define WINDOWS 0
#define LINUX   1

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
# define PLAT_FORM WINDOWS
#elif defined(__linux__)
# define PLAT_FORM LINUX
#endif