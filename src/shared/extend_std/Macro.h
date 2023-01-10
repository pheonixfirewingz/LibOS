#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
//
// File Created by Luke Shore 05.12.2022 uk standard
#include <Cmake.h>
#if __has_include(<winapifamily.h>)
#    include <winapifamily.h>
#endif
#if __has_include(<windows.h>)
#    define WIN32_LEAN_AND_MEAN
#    define NOCOMM
#    include <windows.h>
#endif
#ifndef linux
#    define linux 0
#endif
#ifndef __linux__
#    define __linux__ 0
#endif
#ifndef __linux
#    define __linux 0
#endif
#ifndef __gnu_linux__
#    define __gnu_linux__ 0
#endif
#ifndef __ANDROID__
#    define __ANDROID__ 0
#endif
#ifndef WIN32
#    define WIN32 0
#endif
#ifndef WIN64
#    define WIN64 0
#endif
#ifdef __FreeBSD__
#    define FreeBSD 1
#else
#    define FreeBSD 0
#endif
#ifdef __NetBSD__
#    define NetBSD 1
#else
#    define NetBSD 0
#endif
#ifdef __OpenBSD__
#    define OpenBSD 1
#else
#    define OpenBSD 0
#endif
#ifdef __bsdi__
#    define bsdi 1
#else
#    define bsdi 0
#endif
#ifdef __DragonFly__
#    define DragonFly 1
#else
#    define DragonFly 0
#endif

#ifndef __APPLE__
#    define __APPLE__ 0
#endif
#ifndef __MACH__
#    define __MACH__ 0
#endif
#define IS_LINUX() (linux || __linux__ || __linux || __gnu_linux__) == true
#define IS_ANDROID() __ANDROID__ == true
#define IS_WINDOWS_WIN32() CMAKE_SYSTEM_ISUWP == 0 && (WIN32 == 1 || WIN64 == 1)
#define IS_WINDOWS_UWP() CMAKE_SYSTEM_ISUWP == 1
#define IS_BSD() (FreeBSD || NetBSD || OpenBSD || bsdi || DragonFly) == true
#define IS_MACOS() (__APPLE__ && __MACH__) = true


#if  defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
#    define DEPRICATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#    define DEPRICATED __declspec(deprecated)
#endif