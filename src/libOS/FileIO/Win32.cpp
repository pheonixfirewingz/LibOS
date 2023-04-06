// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Error.h>
#include <libos/FileIO.h>
#define WIN32_LEAN_AND_MEAN
#include <algorithm>
#include <string>
#include <windows.h>

struct losFileHandle_T
{
    HANDLE file_handle = INVALID_HANDLE_VALUE;
    HINSTANCE lib_handle = nullptr;
    bool flip_endianness = false; // Whether the file's endianness should be flipped when reading/writing.
    bool dll_mode = false;        // Whether the file is a dll or not.
    bool unicode_mode = false;    // Whether the file is in Unicode mode.
    bool errored = false;         // Whether an error occurred while operating on the file.
};

// A utility function to get the correct path of the file based on platform and asset path
extern std::string getCorrectPath(_in_ const std::string);

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
losResult losDoseFileExist(_in_ const char *path)
{
    DWORD fileAttributes = GetFileAttributes(getCorrectPath(path).c_str());
    return fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
               ? LOS_SUCCESS
               : LOS_ERROR_MALFORMED_DATA;
}

losResult losOpenFile(_out_ losFileHandle *handle, _in_ const losFileOpenInfo info)
{
    // Create a new losFileHandle object and check if it was successfully created
    if (!(*handle = new losFileHandle_T()))
        return LOS_ERROR_COULD_NOT_INIT;
    if ((info.fileBits & LOS_FILE_BIT_BINARY) != 0)
    {
        std::string lib_str(info.path, 0, info.path_size);
        if (!lib_str.ends_with(".dll"))
            lib_str += ".dll";
        lib_str = getCorrectPath(lib_str);
        (*handle)->dll_mode = true;
        (*handle)->lib_handle = LoadLibrary(lib_str.c_str());
        if ((*handle)->lib_handle == nullptr)
        {
            losPrintLastSystemError();
            return LOS_ERROR_COULD_NOT_INIT;
        }
    }
    else
    {
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
        (*handle)->file_handle = CreateFile(path.c_str(), file_flags, 0, nullptr, file_flags, at_flags, nullptr);
#else
        (*handle)->file_handle = CreateFile(path.c_str(), file_flags, 0, nullptr, create_file_flags, at_flags, nullptr);
#endif
        if ((*handle)->file_handle == INVALID_HANDLE_VALUE)
        {
            (*handle)->errored = true;
            losPrintDebug((std::string("path: ") += path).c_str());
            losPrintLastSystemError();
            return LOS_ERROR_COULD_NOT_INIT;
        }
    }
    // Return success status
    return LOS_SUCCESS;
}

// Closes the file associated with the given handle and releases the resources.
losResult losCloseFile(_in_ losFileHandle handle)
{
    if (!handle->dll_mode)
    {
        CloseHandle(handle->file_handle);
        delete handle;
        handle = nullptr;
    }
    else
    {
        FreeLibrary(handle->lib_handle);
        delete handle;
        handle = nullptr;
    }
    return LOS_SUCCESS;
}

// Reads data from a file.
losResult losReadFile(_in_ losFileHandle handle, _out_ void **data_ptr, _out_ size_t *bytes_read)
{
    // Check if the handle is in an error state
    if (handle->errored || handle->dll_mode)
    {
        *data_ptr = new char[1]{'\0'};
        *bytes_read = 0;
        return LOS_ERROR_MALFORMED_DATA;
    }

    // Get file information using GetFileInformationByHandleEx system call
    FILE_STANDARD_INFO file_info = {0};
    DWORD read_bytes = 0;
    if (!GetFileInformationByHandleEx(handle->file_handle, FileStandardInfo, &file_info, sizeof(FILE_STANDARD_INFO)))
    {
        *data_ptr = new char[1]{'\0'};
        *bytes_read = 0;
        return LOS_ERROR_MALFORMED_DATA;
    }
    if (!handle->unicode_mode)
    {
        *data_ptr = new char[file_info.EndOfFile.QuadPart + 1];
        std::memset(*data_ptr, 0, file_info.EndOfFile.QuadPart + 1);
        if (ReadFile(handle->file_handle, *data_ptr, static_cast<DWORD>(file_info.EndOfFile.QuadPart), &read_bytes,
                     nullptr) == FALSE)
        {
            losPrintLastSystemError();
            return LOS_ERROR_MALFORMED_DATA;
        }
        *bytes_read = read_bytes;
    }
    else
    {
        std::string data(file_info.EndOfFile.QuadPart, 0);
        if (ReadFile(handle->file_handle, &data[0], DWORD(data.size()), &read_bytes, nullptr) == FALSE)
        {
            *data_ptr = new wchar_t[1]{'\0'};
            *bytes_read = 0;
            losPrintLastSystemError();
            return LOS_ERROR_MALFORMED_DATA;
        }
        int utf16_length = MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, nullptr, 0);
        *data_ptr = new wchar_t[utf16_length];
        std::memset(*data_ptr, 0, utf16_length * sizeof(wchar_t));
        MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, (wchar_t *)*data_ptr, utf16_length);
        *bytes_read = utf16_length;
    }
    if (read_bytes != static_cast<DWORD>(file_info.EndOfFile.QuadPart))
        return LOS_ERROR_MALFORMED_DATA;
    // Return success status
    return LOS_SUCCESS;
}

// Writes data to a file.
losResult losWriteFile(_in_ losFileHandle handle, _in_ const void *data_ptr, _in_ const size_t data_size)
{
    // Check if the handle is in an error state
    if (handle->errored || handle->dll_mode)
        return LOS_ERROR_MALFORMED_DATA;
    DWORD bytes_written = 0;
    if (handle->unicode_mode)
    {
        // If the file is in Unicode mode, we need to cast the data pointer to wchar_t* and calculate the byte count
        const wchar_t *dptr = static_cast<const wchar_t *>(data_ptr);
        int len = WideCharToMultiByte(CP_ACP, 0, dptr, -1, nullptr, 0, nullptr, nullptr) - 1;
        std::string bytes_str(len, 0);
        WideCharToMultiByte(CP_ACP, 0, dptr, -1, &bytes_str[0], len, nullptr, nullptr);
        // Call the Unicode version of WriteFile with the wide character buffer and the byte count
        if (WriteFile(handle->file_handle, bytes_str.c_str(), static_cast<DWORD>(len), &bytes_written, nullptr) ==
            FALSE)
        {
            losPrintLastSystemError();
            return LOS_ERROR_MALFORMED_DATA;
        }
        // Check if the bytes written match the expected size
        if (bytes_written != static_cast<DWORD>(len))
            return LOS_ERROR_MALFORMED_DATA;
    }
    else
    {
        // If the file is not in Unicode mode, call the regular version of WriteFile with the data pointer and size
        if (WriteFile(handle->file_handle, data_ptr, static_cast<DWORD>(data_size), &bytes_written, nullptr) == FALSE)
        {
            losPrintLastSystemError();
            return LOS_ERROR_MALFORMED_DATA;
        }
        // Check if the bytes written match the expected size
        if (bytes_written != static_cast<DWORD>(data_size))
            return LOS_ERROR_MALFORMED_DATA;
    }
    // Return success if everything went well
    return LOS_SUCCESS;
}

void *losGetFuncAddress(_in_ const losFileHandle handle, _in_ const char *name)
{
    if (handle->errored || !handle->dll_mode)
        return nullptr;
    return GetProcAddress(handle->lib_handle, name);
}

void losUnicodeToBytes(_in_ const wchar_t *src, _out_ char **dest)
{
    int len = WideCharToMultiByte(CP_ACP, 0, src, -1, nullptr, 0, nullptr, nullptr) - 1;
    *dest = new char[len];
    std::memset(*dest, 0, len * sizeof(char));
    WideCharToMultiByte(CP_ACP, 0, src, -1, &*dest[0], len, nullptr, nullptr);
}

void losBytesToUnicode(_in_ const char *src, _out_ wchar_t **dest)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, nullptr, 0);
    *dest = new wchar_t[len];
    std::memset(*dest, 0, len * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, src, -1, &*dest[0], len);
}