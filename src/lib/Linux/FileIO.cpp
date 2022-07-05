#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include "FileIO.h"
#include <Components/FileIO.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

struct losFileHandle_T
{
    int n_handle;
    std::string path;
    bool flipEndienness = false;
    bool closeAfterDone = false;
};

#define BIT_CASE(bits, flag, e) \
    case bits: {                \
        file_flags |= flag;     \
        flags_used = bits;      \
        extend = e;             \
        break;                  \
    }

losResult losOpenFile(losFileHandle *handle, const losFileOpenInfo &info)
{
    /* FIXME: this check should stop reusing handles already in use
    if (!(*handle))
        return LOS_ERROR_HANDLE_IN_USE;
    */
    (*handle) = new losFileHandle_T();
    int file_flags = 0;
    int flags_used = 0;
    int extend = 0;

    switch (info.fileBits)
    {
        BIT_CASE(LOS_FILE_BIT_CREATE, O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
        BIT_CASE(LOS_FILE_BIT_DELETE_AFTER_CLOSE, 0, 0)
        BIT_CASE(LOS_FILE_BIT_READ, O_RDONLY, 0)
        BIT_CASE(LOS_FILE_BIT_WRITE, O_WRONLY | O_TRUNC, 0)

        BIT_CASE(LOS_FILE_BIT_CREATE | LOS_FILE_BIT_BINARY, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

        BIT_CASE(LOS_FILE_BIT_READ | LOS_FILE_BIT_BINARY, O_RDONLY, 0)
        BIT_CASE(LOS_FILE_BIT_READ | LOS_FILE_BIT_CREATE, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

        BIT_CASE(LOS_FILE_BIT_WRITE | LOS_FILE_BIT_BINARY, O_WRONLY | O_TRUNC, 0)
        BIT_CASE(LOS_FILE_BIT_WRITE | LOS_FILE_BIT_CREATE, O_WRONLY | O_CREAT | O_TRUNC,
                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

        BIT_CASE(LOS_FILE_BIT_READ | LOS_FILE_BIT_BINARY | LOS_FILE_BIT_CREATE, O_RDONLY | O_CREAT,
                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
        BIT_CASE(LOS_FILE_BIT_WRITE | LOS_FILE_BIT_BINARY | LOS_FILE_BIT_CREATE, O_WRONLY | O_CREAT | O_TRUNC,
                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

        BIT_CASE(LOS_FILE_BIT_BINARY | LOS_FILE_BIT_READ | LOS_FILE_BIT_WRITE, O_RDWR | O_TRUNC,0)
        BIT_CASE(LOS_FILE_BIT_CREATE | LOS_FILE_BIT_READ | LOS_FILE_BIT_WRITE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
        BIT_CASE(LOS_FILE_BIT_BINARY | LOS_FILE_BIT_CREATE | LOS_FILE_BIT_READ | LOS_FILE_BIT_WRITE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
    default:
        return LOS_ERROR_INVALID_FLAGS;
    }
    (*handle)->n_handle = open64(getCorrectPath(info.path).c_str(), file_flags, extend);
    if ((*handle)->n_handle < 0)
        return LOS_ERROR_COULD_NOT_INIT;

    (*handle)->path = getCorrectPath(info.path);

    if (flags_used == LOS_FILE_BIT_DELETE_AFTER_CLOSE)
        (*handle)->closeAfterDone = true;

    switch (info.fileBits - flags_used)
    {
    case 0:
        break;
    case LOS_FILE_BIT_DELETE_AFTER_CLOSE: {
        (*handle)->closeAfterDone = true;
        break;
    }
    case LOS_FILE_BIT_FLIP_ENDIEN: {
        (*handle)->flipEndienness = true;
        break;
    }
    case LOS_FILE_BIT_DELETE_AFTER_CLOSE | LOS_FILE_BIT_FLIP_ENDIEN: {
        (*handle)->closeAfterDone = true;
        (*handle)->flipEndienness = true;
        break;
    }
    default:
        return LOS_ERROR_INVALID_FLAGS;
    }
    return LOS_SUCCESS;
}

losResult losCloseFile(losFileHandle handle)
{
    if (handle->closeAfterDone)
        remove(handle->path.c_str());

    close(handle->n_handle);
    handle = NULL;
    return LOS_SUCCESS;
}

losResult losReadFile(losFileHandle handle, void **data_ptr, size* data_size)
{
    struct stat sb{};
    if (fstat(handle->n_handle, &sb) < 0)
    {
        perror("system error");
        losCloseFile(handle);
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    }
    *data_size = (sb.st_size) * sizeof(char);
    *data_ptr = malloc(*data_size);
    ssize_t bytesRead = 0;
    if ((bytesRead = pread64(handle->n_handle, *data_ptr, sb.st_size, 0)) < 0)
    {
        perror("system error");
        losCloseFile(handle);
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    }

    if (data_ptr != nullptr)
        *data_size = bytesRead;

    return LOS_SUCCESS;
}

losResult losWriteFile(losFileHandle handle, const void *data, const size data_size)
{
    if (pwrite64(handle->n_handle, data, data_size, 0) < 0)
    {
        perror("system error");
        losCloseFile(handle);
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    }
    return LOS_SUCCESS;
}

std::string linuxGetCurrentPath()
{
    char result[4096];
    (void)readlink("/proc/self/exe", result, 4096);
    return dirname(result);
}
#endif