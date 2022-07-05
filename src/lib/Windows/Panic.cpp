#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 1
#include <Components/Defines.h>
#include <cstdio>
#include <cstdlib>

inline void lib_panic(const char* msg)
{
    printf("OS LIB PANIC: %s\n", msg);
    exit(-1);
}
#endif