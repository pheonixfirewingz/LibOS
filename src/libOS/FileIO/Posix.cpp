// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <cstring>
#include <fcntl.h>
#include <libgen.h>
#include <libos/FileIO.h>
#include <linux/limits.h>
#include <ranges>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

struct losFileHandle_T
{
    int32_t n_handle;
    std::string path;
    bool flipEndianness = false;
    bool closeAfterDone = false;
};

#define BIT_CASE(bits, flag, e) \
    case bits: {                \
        file_flags |= flag;     \
        flags_used = bits;      \
        extend = e;             \
        break;                  \
    }
extern std::string getCorrectPath(const char *);

std::string binaryPath()
{
    char result[4096];
    if (readlink("/proc/self/exe", result, 4096) < 0)
        return "";
    return dirname(result);
}

std::string platformGetCurrentPath()
{
    // FIXME: doing this is bad and is a nasty hack
    static std::string ret = binaryPath();
    return ret;
}

std::vector<std::string> iSplit(std::string s, char delimiter) noexcept
{
    std::vector<std::string> ret;
    for (const auto word : std::views::split(s, delimiter))
        ret.push_back(std::string(word.begin(), word.end()));
    return ret;
}

std::vector<std::string> platformSplit(std::string path) noexcept
{
    std::vector<std::string> ret;
    auto s = iSplit(path.c_str(), '/');
    for (auto str : s)
    {
        if (!str.contains("\\"))
            ret.push_back(str);
        else
        {
            auto t = iSplit(str, '\\');
            for (auto r : t)
                ret.push_back(r);
        }
    }
    return ret;
}

losResult platformOpenFile(losFileHandle *handle, const losFileOpenInfo info)
{
    if (!(*handle = new losFileHandle_T()))
        return LOS_ERROR_COULD_NOT_INIT;

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

        BIT_CASE(LOS_FILE_BIT_BINARY | LOS_FILE_BIT_READ | LOS_FILE_BIT_WRITE, O_RDWR | O_TRUNC, 0)
        BIT_CASE(LOS_FILE_BIT_CREATE | LOS_FILE_BIT_READ | LOS_FILE_BIT_WRITE, O_RDWR | O_CREAT,
                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
        BIT_CASE(LOS_FILE_BIT_BINARY | LOS_FILE_BIT_CREATE | LOS_FILE_BIT_READ | LOS_FILE_BIT_WRITE, O_RDWR | O_CREAT,
                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
    default:
        return LOS_ERROR_INVALID_FLAGS;
    }
    const std::string path_corrected = getCorrectPath(info.path);
    if (((*handle)->n_handle = open64(path_corrected.c_str(), file_flags, extend)) < 0)
    {
        puts("LIBOS ERROR: Could not open file");
        puts(path_corrected.c_str());
        puts("\n");
        return LOS_ERROR_COULD_NOT_INIT;
    }
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
    case LOS_FILE_BIT_FLIP_ENDIAN: {
        (*handle)->flipEndianness = true;
        break;
    }
    case LOS_FILE_BIT_DELETE_AFTER_CLOSE | LOS_FILE_BIT_FLIP_ENDIAN: {
        (*handle)->closeAfterDone = true;
        (*handle)->flipEndianness = true;
        break;
    }
    default:
        return LOS_ERROR_INVALID_FLAGS;
    }
    return LOS_SUCCESS;
}

losResult platformCloseFile(losFileHandle handle)
{
    if (handle->closeAfterDone)
        remove(handle->path.c_str());

    close(handle->n_handle);
    handle = NULL;
    return LOS_SUCCESS;
}

losResult platformReadFile(losFileHandle handle, void **data_ptr, data_size_t *bytes_read)
{
    struct stat sb
    {
    };
    if (fstat(handle->n_handle, &sb) < 0)
    {
        perror("system error");
        losCloseFile(handle);
        return LOS_ERROR_MALFORMED_DATA;
    }
    *data_ptr = malloc(sb.st_size);
    memset(*data_ptr, 0, sb.st_size);
    sdata_size_t bytesRead = 0;
    if ((bytesRead = pread64(handle->n_handle, *data_ptr, sb.st_size, 0)) < 0)
    {
        perror("system error");
        losCloseFile(handle);
        return LOS_ERROR_MALFORMED_DATA;
    }
    *bytes_read = bytesRead;
    return LOS_SUCCESS;
}

losResult platformWriteFile(losFileHandle handle, const void *data, const data_size_t data_data_size)
{
    if (pwrite64(handle->n_handle, data, data_data_size, 0) < 0)
    {
        perror("system error");
        losCloseFile(handle);
        return LOS_ERROR_MALFORMED_DATA;
    }
    return LOS_SUCCESS;
}