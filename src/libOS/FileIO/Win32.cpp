#include <Cmake.h>
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Defines.h>
#include <libos/FileIO.h>
#include <string>
#include <ranges>
#include <string_view>
#include <vector>
#include <atomic>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct losFileHandle_T
{
    HANDLE fileHandle = NULL;
    bool flipEndianness = false;
    bool closeAfterDone = false;
};
#define BIT_CASE(bits, flag, c_flag, at) \
    case bits: {                         \
        file_flags = flag;               \
        create_file_flags = c_flag;      \
        flags_used = bits;               \
        at_flags = at;                   \
        break;                           \
    }

extern std::string getCorrectPath(const char *);

#pragma warning(push)
#pragma warning(disable : 4172)
std::string platformGetCurrentPath()
{
    char path[260]{};
    GetModuleFileNameA(NULL, (LPSTR)&path, 260);
    return path;
}
#pragma warning(pop)

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
    for (auto& str : s)
    {
        if (!str.contains("\\"))
            ret.push_back(str);
        else
        {
            auto t = iSplit(str, '\\');
            for (auto& r : t)
                ret.push_back(r);
        }
    }
    return ret;
}

 LPSTR GetFormattedMessage(DWORD pMessage)
{
    LPSTR pBuffer = NULL;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, (LPCVOID)pMessage, 0, 0, (LPSTR)&pBuffer, 0,NULL);
    return pBuffer;
}

losResult platformOpenFile(losFileHandle *handle, const losFileOpenInfo info)
{
    if (!(*handle))
        return LOS_ERROR_COULD_NOT_INIT;

    *handle = new losFileHandle_T();
    UINT file_flags = 0;
    UINT create_file_flags = 0;
    UINT flags_used = 0;
    UINT at_flags = 0;
    switch (info.fileBits)
    {
        BIT_CASE(LOS_FILE_BIT_CREATE, GENERIC_READ, CREATE_NEW, FILE_ATTRIBUTE_NORMAL)
        BIT_CASE(LOS_FILE_BIT_DELETE_AFTER_CLOSE, GENERIC_READ, OPEN_EXISTING,
                 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE)
        BIT_CASE(LOS_FILE_BIT_READ, GENERIC_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL)
        BIT_CASE(LOS_FILE_BIT_WRITE, GENERIC_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL)

        BIT_CASE(LOS_FILE_BIT_CREATE | LOS_FILE_BIT_BINARY, GENERIC_READ, CREATE_NEW, FILE_ATTRIBUTE_NORMAL)

        BIT_CASE(LOS_FILE_BIT_READ | LOS_FILE_BIT_BINARY, GENERIC_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL)
        BIT_CASE(LOS_FILE_BIT_READ | LOS_FILE_BIT_CREATE, GENERIC_READ, CREATE_NEW, FILE_ATTRIBUTE_NORMAL)

        BIT_CASE(LOS_FILE_BIT_WRITE | LOS_FILE_BIT_BINARY, GENERIC_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL)
        BIT_CASE(LOS_FILE_BIT_WRITE | LOS_FILE_BIT_CREATE, GENERIC_WRITE, CREATE_NEW, FILE_ATTRIBUTE_NORMAL)

        BIT_CASE(LOS_FILE_BIT_READ | LOS_FILE_BIT_BINARY | LOS_FILE_BIT_CREATE, GENERIC_READ, CREATE_NEW,
                 FILE_ATTRIBUTE_NORMAL)
        BIT_CASE(LOS_FILE_BIT_WRITE | LOS_FILE_BIT_BINARY | LOS_FILE_BIT_CREATE, GENERIC_WRITE, CREATE_NEW,
                 FILE_ATTRIBUTE_NORMAL)
    default:
        return LOS_ERROR_INVALID_FLAGS;
    }
    auto path = getCorrectPath(info.path);
#if DEBUG_MODE
    printf("LIBOS - FILE PATH: %s\n", path.c_str());
#endif
#if UNICODE
    std::string data_small = getCorrectPath(info.path);
    std::wstring data = std::wstring(data_small.begin(), data_small.end());
    (*handle)->fileHandle = CreateFile(path.c_str(), file_flags, 0, nullptr, create_file_flags, at_flags, nullptr);
#else
    (*handle)->fileHandle = CreateFile(path.c_str(), file_flags, 0, nullptr, create_file_flags, at_flags, nullptr);
#endif
    if ((*handle)->fileHandle == INVALID_HANDLE_VALUE)
    {
        puts(GetFormattedMessage(GetLastError()));
        return LOS_ERROR_COULD_NOT_INIT;
    }

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
    CloseHandle(handle->fileHandle);
    delete handle;
    return LOS_SUCCESS;
}

std::atomic<DWORD> BytesTransferred;
losResult platformReadFile(losFileHandle handle, void **data_ptr, data_size_t *bytes_read)
{
    OVERLAPPED overlapped = {0};
    FILE_STANDARD_INFO file_info = {0};

    if (GetFileInformationByHandleEx(handle->fileHandle, FileStandardInfo, &file_info, sizeof(FILE_STANDARD_INFO)) == 0)
    {
        losCloseFile(handle);
        return LOS_ERROR_MALFORMED_DATA;
    }

    *data_ptr = new char[file_info.EndOfFile.QuadPart + 1];
    ZeroMemory(*data_ptr, file_info.EndOfFile.QuadPart + 1);

    if (ReadFileEx(handle->fileHandle, *data_ptr, static_cast<DWORD>(file_info.EndOfFile.QuadPart), &overlapped,
                   [](__in DWORD, __in DWORD dwNumberOfBytesTransfered, __in LPOVERLAPPED) {
                       BytesTransferred = dwNumberOfBytesTransfered;
                   }) == 0)
    {
        losCloseFile(handle);
        return LOS_ERROR_MALFORMED_DATA;
    }

    if (data_ptr != nullptr)
        *bytes_read = file_info.EndOfFile.QuadPart;

    BytesTransferred = 0;
    return LOS_SUCCESS;
}

losResult platformWriteFile(losFileHandle handle, const void *data_ptr, const data_size_t data_size)
{
    DWORD dwBytesWrite = 0;
    if (WriteFile(handle->fileHandle, data_ptr, (DWORD)data_size, &dwBytesWrite, nullptr) == 0)
    {
        losCloseFile(handle);
        return LOS_ERROR_MALFORMED_DATA;
    }
    (void)dwBytesWrite;
    return LOS_SUCCESS;
}