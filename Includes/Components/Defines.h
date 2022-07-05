#pragma once
#if _WIN64
#    define EXPORT_DLL extern "C" __declspec(dllexport)
#else
#    define EXPORT_DLL extern "C"
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef uint16 losUnicode;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef unsigned long long size;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long int64;
typedef signed long long ssize;

typedef float float32;
typedef double float64;

#define DEFINE_HANDLE(object) typedef struct object##_T *object;

EXPORT_DLL void lib_panic(const char*);
#define prog_panic(x) lib_panic(x)

typedef struct losSize
{
    uint64 width;
    uint64 height;
} losSize;

typedef enum losResult : uint8
{
    LOS_SUCCESS,
    LOS_ERROR_HANDLE_IN_USE,
    LOS_ERROR_COULD_NOT_GET_CORRECT_DATA,
    LOS_ERROR_COULD_NOT_INIT,
    LOS_ERROR_COULD_NOT_DESTORY,
    LOS_ERROR_INVALID_FLAGS,
    LOS_ERROR_HANDLE_LOSSED,
    LOS_ERROR_FEATURE_NOT_IMPLEMENTED,
    // API CUSTOMS
    // NETWORK
    LOS_NET_IO_DOMAIN_NOT_FOUND,
    LOS_NET_IO_CONNECTION_REFUSED,
    LOS_NET_IO_CONNECTION_CLOSED_SERVER_END,
    // WINDOW
    LOS_WINDOW_CLOSE,
    //ThreadGuard
    LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD
} losResult;
