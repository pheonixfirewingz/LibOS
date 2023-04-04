// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Error.h>
#include <cerrno>
#include <cstring>
#include <cstdio>


void losPrintLastSystemError()
{
    fprintf(stderr,"SYSTEM ERROR: %s\n", strerror(errno));
}

void losPrintInfo(const char *str)
{
    printf("PROGRAM INFO: %s\n", str);
}

void losPrintDebug(const char *str)
{
    printf("PROGRAM DEBUG: %s\n", str);
}

void losPrintError(const char *str)
{
    fprintf(stderr,"PROGRAM ERROR: %s\n", str);
}