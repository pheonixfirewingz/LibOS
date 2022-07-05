#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 1
#include "../IFileIO.h"
#include "windows_link.h"
#include <Components/FileIO.h>
#include <atomic>

struct losFileHandle_T
{
    HANDLE fileHandle;
    bool flipEndienness = false;
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

losResult losOpenFile(losFileHandle *handle, const losFileOpenInfo &info)
{
    if (!(*handle))
        return LOS_ERROR_HANDLE_IN_USE;

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

    (*handle)->fileHandle =
        CreateFile(getCorrectPath(info.path).c_str(), file_flags, 0, nullptr, create_file_flags, at_flags, nullptr);

    if ((*handle)->fileHandle == INVALID_HANDLE_VALUE)
        return LOS_ERROR_COULD_NOT_INIT;

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

std::atomic<DWORD> BytesTransferred;
losResult losReadFile(losFileHandle handle, void **data_ptr, size* data_size)
{
    OVERLAPPED overlapped = {0};
    FILE_STANDARD_INFO file_info = {0};

    if(GetFileInformationByHandleEx(handle->fileHandle,FileStandardInfo,&file_info,sizeof(FILE_STANDARD_INFO))  == 0)
    {
        losCloseFile(handle);
        return LOS_ERROR_HANDLE_LOSSED;
    }

    *data_ptr = new char[file_info.EndOfFile.QuadPart + 1];
    ZeroMemory(*data_ptr, file_info.EndOfFile.QuadPart + 1);

    if (ReadFileEx(handle->fileHandle, *data_ptr, static_cast<DWORD>(file_info.EndOfFile.QuadPart), &overlapped, 
    [](__in  DWORD,__in  DWORD dwNumberOfBytesTransfered,__in  LPOVERLAPPED)
    {
      BytesTransferred = dwNumberOfBytesTransfered;
    }) == 0)
    {
        losCloseFile(handle);
        return LOS_ERROR_HANDLE_LOSSED;
    }

    if (data_ptr != nullptr)
        *data_size = file_info.EndOfFile.QuadPart;
        
    //BytesTransferred = 0;
    return LOS_SUCCESS;
 }

losResult losWriteFile(losFileHandle handle, const void *data_ptr, const size data_size)
{
    DWORD dwBytesWrite = 0;
    if (WriteFile(handle->fileHandle, data_ptr, (DWORD)data_size, &dwBytesWrite, nullptr) == 0)
    {
        losCloseFile(handle);
        return LOS_ERROR_HANDLE_LOSSED;
    }
    (void)dwBytesWrite;
    return LOS_SUCCESS;
}

losResult losCloseFile(losFileHandle handle)
{
    CloseHandle(handle->fileHandle);
    delete handle;
    return LOS_SUCCESS;
}

std::string winGetCurrentPath()
{
    std::string path;
    for (DWORD str_size = 0; str_size < MAX_PATH; str_size++)
    {
        path += " ";
    }
    GetModuleFileName(NULL, (LPSTR)path.c_str(), MAX_PATH);
    return path;
}
#endif