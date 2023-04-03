// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/FileIO.h>
#define WIN32_LEAN_AND_MEAN
#include <algorithm>
#include <iostream>
#include <string>
#include <windows.h>

struct losFileHandle_T
{
    HANDLE fileHandle = NULL;
    bool flip_endianness = false; // Whether the file's endianness should be flipped when reading/writing.
    bool unicode_mode = false;    // Whether the file is in Unicode mode.
    bool errored = false;         // Whether an error occurred while operating on the file.
};

// A utility function to get the correct path of the file based on platform and asset path
extern std::string getCorrectPath(const std::string);

#pragma warning(push)
#pragma warning(disable : 4172)
// Returns the current binary directory as a string.
std::string platformGetCurrentPath()
{
    // Allocate a buffer to store the path.
    char buf[MAX_PATH];
    // Read the symbolic link /proc/self/exe into the buffer.
    // This returns the absolute path of the current binary file.
    GetModuleFileNameA(NULL, (LPSTR)&buf, sizeof(buf) - 1);
    // Convert the buffer to a C++ string.
    std::string path(buf);
    // Strip the binary filename from the end of the path, leaving only the directory.
    // Find the last occurrence of '/' in the string.
    size_t pos = path.rfind('/');
    if (pos != std::string::npos)
        // Erase everything after the last '/' character.
        path.erase(pos);
    pos = path.rfind('\\');
    if (pos != std::string::npos)
    {
        // Erase everything after the last '\\' character.
        path.erase(pos);
        std::replace(path.begin(), path.end(), '\\', '/');
    }
    // Return the resulting path string.
    return path;
}
#pragma warning(pop)

LPSTR GetFormattedMessage(LPCVOID pMessage)
{
    LPSTR pBuffer = NULL;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, pMessage, 0, 0, (LPSTR)&pBuffer, 0,
                  NULL);
    return pBuffer;
}

losResult losDoseFileExist(const char *path)
{
    DWORD fileAttributes = GetFileAttributes(getCorrectPath(path).c_str());
    return fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
               ? LOS_SUCCESS
               : LOS_ERROR_MALFORMED_DATA;
}

losResult losOpenFile(losFileHandle *handle, const losFileOpenInfo info)
{
    // Create a new losFileHandle object and check if it was successfully created
    if (!(*handle = new losFileHandle_T()))
        return LOS_ERROR_COULD_NOT_INIT;

    // Initialize file flags based on fileBits field of losFileOpenInfo structure
    DWORD at_flags = FILE_ATTRIBUTE_NORMAL;
    DWORD create_file_flags = 0;
    DWORD file_flags = 0;
    if ((info.fileBits & LOS_FILE_BIT_CREATE) != 0)
        create_file_flags = CREATE_NEW;
    else
        create_file_flags = OPEN_EXISTING;

    if ((info.fileBits & LOS_FILE_BIT_READ) != 0 && (info.fileBits & LOS_FILE_BIT_WRITE) != 0)
        file_flags |= GENERIC_READ | GENERIC_WRITE;
    else if ((info.fileBits & LOS_FILE_BIT_READ) != 0)
        file_flags |= GENERIC_READ;
    else if ((info.fileBits & LOS_FILE_BIT_WRITE) != 0)
        file_flags |= GENERIC_WRITE;
    else
        file_flags |= GENERIC_READ;

    // Set handle properties based on fileBits field of losFileOpenInfo structure
    if ((info.fileBits & LOS_FILE_BIT_DELETE_AFTER_CLOSE) != 0)
        at_flags |= FILE_FLAG_DELETE_ON_CLOSE;
    if ((info.fileBits & LOS_FILE_BIT_FLIP_ENDIAN) != 0)
        (*handle)->flip_endianness = true;
    if ((info.fileBits & LOS_FILE_BIT_UNICODE) != 0)
        (*handle)->unicode_mode = true;

    // Set path member of losFileHandle object to the correct path for the file
    std::string path = getCorrectPath(std::string(info.path, 0, info.path_size).c_str());
    // Open the file with the specified flags and file mode, and store the file descriptor in n_handle member of
    // losFileHandle object
#if UNICODE
    std::wstring data = std::wstring(path.begin(), path.end());
    (*handle)->fileHandle = CreateFile(path.c_str(), file_flags, 0, nullptr, file_flags, at_flags, nullptr);
#else
    (*handle)->fileHandle = CreateFile(path.c_str(), file_flags, 0, nullptr, create_file_flags, at_flags, nullptr);
#endif
    if ((*handle)->fileHandle == INVALID_HANDLE_VALUE)
    {
        (*handle)->errored = true;
#if IS_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4312)
#endif
        std::cerr << "system error: " << GetFormattedMessage(reinterpret_cast<LPCVOID>(GetLastError())) << std::endl;
#if IS_MSVC
#    pragma warning(pop)
#endif
        std::cerr << "path: " << path << std::endl;
        return LOS_ERROR_COULD_NOT_INIT;
    }
    // Return success status
    return LOS_SUCCESS;
}

losResult losCloseFile(losFileHandle handle)
{
    if (handle)
    {
        CloseHandle(handle->fileHandle);
        delete handle;
    }
    return LOS_SUCCESS;
}

// Reads data from a file.
losResult losReadFile(losFileHandle handle, void **data_ptr, data_size_t *bytes_read)
{
    // Check if the handle is in an error state
    if (handle->errored)
        return LOS_ERROR_MALFORMED_DATA;
    // Get file information using GetFileInformationByHandleEx system call
    OVERLAPPED overlapped = {0};
    FILE_STANDARD_INFO file_info = {0};
    if (GetFileInformationByHandleEx(handle->fileHandle, FileStandardInfo, &file_info, sizeof(FILE_STANDARD_INFO)) == 0)
    {
        losCloseFile(handle);
        return LOS_ERROR_MALFORMED_DATA;
    }
    if (!handle->unicode_mode)
    {
        *data_ptr = new char[file_info.EndOfFile.QuadPart + 1];
        ZeroMemory(*data_ptr, file_info.EndOfFile.QuadPart + 1);

        if (ReadFileEx(handle->fileHandle, *data_ptr, static_cast<DWORD>(file_info.EndOfFile.QuadPart), &overlapped,
                       [](__in DWORD, __in DWORD, __in LPOVERLAPPED) {}) == 0)
        {
            losCloseFile(handle);
            return LOS_ERROR_MALFORMED_DATA;
        }

        if (data_ptr != nullptr)
            *bytes_read = file_info.EndOfFile.QuadPart;
    }
    else
    {
        *data_ptr = new char8_t[file_info.EndOfFile.QuadPart + 1];
        ZeroMemory(*data_ptr, file_info.EndOfFile.QuadPart + 1);

        if (ReadFileEx(handle->fileHandle, *data_ptr, static_cast<DWORD>(file_info.EndOfFile.QuadPart), &overlapped,
                       [](__in DWORD, __in DWORD, __in LPOVERLAPPED) {}) == 0)
        {
            losCloseFile(handle);
            return LOS_ERROR_MALFORMED_DATA;
        }

        if (data_ptr != nullptr)
            *bytes_read = file_info.EndOfFile.QuadPart;
    }
    // Return success status
    return LOS_SUCCESS;
}

losResult losWriteFile(losFileHandle handle, const void *data_ptr, const data_size_t data_size)
{
    // Check if the handle is in an error state
    if (handle->errored)
        return LOS_ERROR_MALFORMED_DATA;
    if (!handle->unicode_mode)
    {
        DWORD dwBytesWrite = 0;
        if (WriteFile(handle->fileHandle, data_ptr, (DWORD)data_size, &dwBytesWrite, nullptr) == 0)
        {
            losCloseFile(handle);
            return LOS_ERROR_MALFORMED_DATA;
        }
        (void)dwBytesWrite;
    }
    else
    {
        DWORD dwBytesWrite = 0;
        if (WriteFile(handle->fileHandle, data_ptr, (DWORD)data_size, &dwBytesWrite, nullptr) == 0)
        {
            losCloseFile(handle);
            return LOS_ERROR_MALFORMED_DATA;
        }
        (void)dwBytesWrite;
    }
    return LOS_SUCCESS;
}