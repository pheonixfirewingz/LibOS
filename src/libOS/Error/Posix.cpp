// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <libos/Error.h>

void losPrintLastSystemError()
{
    fprintf(stderr, "SYSTEM ERROR: %s\n", strerror(errno));
}

void losPrintInfo(const char *str...)
{
    char *ptr = new char[std::strlen(str) + 1];
    va_list list;
    va_start(list, str);
    sprintf(ptr, str, list);
    va_end(list);
    puts(ptr);
    delete[] ptr;
}

void losPrintDebug([[maybe_unused]] const char *str...)
{
#if DEBUG_MODE
    losPrintInfo(str);
#endif
}

void losPrintError(const char *str...)
{
    char *ptr = nullptr;
    va_list list;
    va_start(list, str);
    sprintf(ptr, str, list);
    va_end(list);
    puts(ptr);
}