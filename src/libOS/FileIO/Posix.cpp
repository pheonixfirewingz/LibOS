// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <cstring>
#include <dlfcn.h>
#include <fcntl.h>
#include <iconv.h>
#include <libos/Error.h>
#include <libos/FileIO.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

size_t u8strlen(const char *utf8Str)
{
    size_t utf32Len = 0;
    const char *ptr = utf8Str;
    while (*ptr != '\0')
    {
        // Determine the number of bytes in the current UTF-8 character
        int numBytes = 0;
        if ((*ptr & 0x80) == 0x00)
            numBytes = 1;
        else if ((*ptr & 0xE0) == 0xC0)
            numBytes = 2;
        else if ((*ptr & 0xF0) == 0xE0)
            numBytes = 3;
        else if ((*ptr & 0xF8) == 0xF0)
            numBytes = 4;
        else
            throw std::runtime_error("Invalid UTF-8 sequence");
        ptr += numBytes;
        utf32Len++;
    }

    return utf32Len;
}

std::wstring u8convert(const char *utf8Str)
{
    std::wstring result;
    const char *ptr = utf8Str;
    while (*ptr != '\0')
    {
        // Determine the number of bytes in the current UTF-8 character
        int numBytes = 0;
        if ((*ptr & 0x80) == 0x00)
        {
            // 1-byte character
            numBytes = 1;
            result += static_cast<wchar_t>((*ptr));
        }
        else if ((*ptr & 0xE0) == 0xC0)
        {
            // 2-byte character
            numBytes = 2;
            wchar_t wch = ((*ptr & 0x1F) << 6) | (*(ptr + 1) & 0x3F);
            result += wch;
        }
        else if ((*ptr & 0xF0) == 0xE0)
        {
            // 3-byte character
            numBytes = 3;
            wchar_t wch = ((*ptr & 0x0F) << 12) | ((*(ptr + 1) & 0x3F) << 6) | (*(ptr + 2) & 0x3F);
            result += wch;
        }
        else if ((*ptr & 0xF8) == 0xF0)
        {
            // 4-byte character
            numBytes = 4;
            wchar_t wch =
                ((*ptr & 0x07) << 18) | ((*(ptr + 1) & 0x3F) << 12) | ((*(ptr + 2) & 0x3F) << 6) | (*(ptr + 3) & 0x3F);
            result += wch;
        }
        else
        {
            throw std::runtime_error("Invalid UTF-8 sequence");
        }
        ptr += numBytes;
    }

    return result;
}

class Converter
{
    const iconv_t cd;

  public:
    Converter(const char *F, const char *T)
        : cd(iconv_open(T, F))
    {
        if (cd == reinterpret_cast<iconv_t>(-1))
            throw std::runtime_error("Failed to open iconv");
    }

    std::string convert(char *input, size_t len) const
    {
        size_t out = len * sizeof(wchar_t);
        size_t in = len * sizeof(wchar_t);
        char *out_buf = new char[out];
        char *out_ptr = out_buf;
        size_t result = 0;
        while (in > 0)
        {
            result = iconv(cd, &input, &in, &out_ptr, &out);
            if (result == static_cast<size_t>(-1))
            {
                delete[] out_buf;
                throw std::runtime_error("Conversion failed");
            }
        }

        size_t output_len = (out_ptr - out_buf);
        std::string output(out_buf, output_len);
        delete[] out_buf;
        return output;
    }

    ~Converter()
    {
        iconv_close(cd);
    }
};
// This struct represents a handle to a file in the LOS filesystem.
struct losFileHandle_T
{
    int32_t n_handle;              // The file descriptor of the file.
    void *lib_handle;              // The handle to the a library
    std::string path;              // The absolute path to the file.
    bool flip_endianness = false;  // Whether the file's endianness should be flipped when reading/writing.
    bool unicode_mode = false;     // Whether the file is in Unicode mode.
    bool dll_mode = false;         // Whether the file is a dll or not.
    bool close_after_done = false; // Whether the file should be closed after the next read/write operation.
    bool errored = false;          // Whether an error occurred while operating on the file.
};

// A utility function to get the correct path of the file based on platform and asset path
extern std::string getCorrectPath(_in_ const std::string);

// Returns the current binary directory as a string.
std::string platformGetCurrentPath()
{
    // Allocate a buffer to store the path.
    char buf[PATH_MAX];
    // Read the symbolic link /proc/self/exe into the buffer.
    // This returns the absolute path of the current binary file.
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);

    // Check for errors.
    if (len == -1)
        // readlink failed, return an empty string.
        return "";

    // Add a null terminator to the end of the buffer to make it a valid C string.
    buf[len] = '\0';
    // Convert the buffer to a C++ string.
    std::string path(buf);
    // Strip the binary filename from the end of the path, leaving only the directory.
    // Find the last occurrence of '/' in the string.
    size_t pos = path.rfind('/');
    if (pos != std::string::npos)
        // Erase everything after the last '/' character.
        path.erase(pos);
    // Return the resulting path string.
    return path;
}

std::string platformGetHomePath()
{
    return std::getenv("HOME") != nullptr ? std::getenv("HOME") : "";
}

// Returns true if the specified file exists, false otherwise.
losResult losDoseFileExist(_in_ const char *path)
{
    // Use the getCorrectPath function to get the corrected file path.
    std::string corrected_path = getCorrectPath(path);
    // Check if the file exists by calling access with the F_OK mode.
    if (access(corrected_path.c_str(), F_OK) == -1)
        // access failed, return an error code.
        return LOS_ERROR_MALFORMED_DATA;
    // access succeeded, return a success code.
    return LOS_SUCCESS;
}

std::string getParentDirectory(const std::string &filePath)
{
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash != std::string::npos)
    {
        return filePath.substr(0, lastSlash);
    }
    return "";
}

losResult losOpenFile(_out_ losFileHandle *handle, _in_ const losFileOpenInfo info)
{
    // Create a new losFileHandle object and check if it was successfully created
    if (!(*handle = new losFileHandle_T()))
        return LOS_ERROR_COULD_NOT_INIT;
    if ((info.fileBits & LOS_FILE_BIT_BINARY) != 0)
    {
        std::string lib_str(info.path, 0, info.path_size);
        if (!lib_str.ends_with(".so"))
            lib_str += ".so";
        lib_str = getCorrectPath(lib_str);
        (*handle)->dll_mode = true;
        (*handle)->lib_handle = dlopen(lib_str.c_str(), RTLD_NOW | RTLD_LOCAL);
        if ((*handle)->lib_handle == nullptr)
        {
            (*handle)->errored = true;
            losPrintLastSystemError();
            return LOS_ERROR_COULD_NOT_INIT;
        }
    }
    else
    {
        // Create a new losFileHandle object and check if it was successfully created
        if (!(*handle = new losFileHandle_T()))
            return LOS_ERROR_COULD_NOT_INIT;

        // Initialize file flags based on fileBits field of losFileOpenInfo structure
        int file_flags = 0;
        if ((info.fileBits & LOS_FILE_BIT_CREATE) != 0)
            file_flags |= O_CREAT;
        if ((info.fileBits & LOS_FILE_BIT_READ) != 0 && (info.fileBits & LOS_FILE_BIT_WRITE) != 0)
            file_flags |= O_RDWR;
        else if ((info.fileBits & LOS_FILE_BIT_READ) != 0)
            file_flags |= O_RDONLY;
        else if ((info.fileBits & LOS_FILE_BIT_WRITE) != 0)
            file_flags |= O_WRONLY | O_TRUNC;

        // Set handle properties based on fileBits field of losFileOpenInfo structure
        if ((info.fileBits & LOS_FILE_BIT_DELETE_AFTER_CLOSE) != 0)
            (*handle)->close_after_done = true;
        if ((info.fileBits & LOS_FILE_BIT_FLIP_ENDIAN) != 0)
            (*handle)->flip_endianness = true;
        if ((info.fileBits & LOS_FILE_BIT_UNICODE) != 0)
            (*handle)->unicode_mode = true;

        // Set path member of losFileHandle object to the correct path for the file
        (*handle)->path = getCorrectPath(std::string(info.path, 0, info.path_size).c_str());
        // Create the Perant Dir if not created
        if ((info.fileBits & LOS_FILE_BIT_CREATE) != 0)
        {
            std::string dir = getParentDirectory((*handle)->path);
            if (losDoseFileExist(dir.c_str()) != LOS_SUCCESS)
            {
                int mkdirResult = mkdir(dir.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                if (mkdirResult != 0)
                {
                    // Handle error if directory creation fails
                    losPrintDebug((std::string("path: ") + dir).c_str());
                    losPrintLastSystemError();
                    return LOS_ERROR_COULD_NOT_INIT;
                }
            }
        }

        // Open the file with the specified flags and file mode, and store the file descriptor in n_handle member of
        // losFileHandle object
        if (((*handle)->n_handle = open64((*handle)->path.c_str(), file_flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) <
            0)
        {
            (*handle)->errored = true;
            losPrintDebug((std::string("path: ") + (*handle)->path).c_str());
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
        if (handle->close_after_done)     // If the file should be deleted
            remove(handle->path.c_str()); // Delete the file

        close(handle->n_handle);          // Close the file handle
        handle = nullptr;
    }
    else
    {
        if (!handle->errored)
        {
            dlclose(handle->lib_handle);
        }
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
    // Get file information using fstat system call
    struct stat sb
    {
    };
    if (fstat(handle->n_handle, &sb) < 0)
    {
        // Print system error message
        losPrintLastSystemError();
        // Return error status
        return LOS_ERROR_MALFORMED_DATA;
    }

    if (handle->unicode_mode)
    {
        // Create a u8string to store the file contents
        std::string str(sb.st_size, 0);
        // Variable to hold the number of bytes read
        ssize_t bytesRead = 0;
        // Read the file using pread64 system call
        if ((bytesRead = pread64(handle->n_handle, str.data(), str.size(), 0)) < 0)
        {
            // Set the error flag in the handle
            handle->errored = true;
            // Print system error message
            losPrintLastSystemError();
            // Return error status
            return LOS_ERROR_MALFORMED_DATA;
        }
        std::wstring w_data = u8convert(str.c_str());
        // Allocate memory for the data to be read
        *data_ptr = new wchar_t[w_data.size() + 1]; // Add 1 for the null terminator
        // Initialize the allocated memory to zero
        memset(*data_ptr, 0, (w_data.size() + 1) * sizeof(wchar_t));
        // Multiply by sizeof(wchar_t) to get size in bytes
        // Copy the wide string data including the null terminator
        memcpy(*data_ptr, w_data.c_str(), (w_data.size() + 1) * sizeof(wchar_t));

        // Set the number of bytes read
        *bytes_read = u8strlen(str.c_str());
        if (bytesRead != sb.st_size)
            return LOS_ERROR_MALFORMED_DATA;
    }
    else
    {
        // Allocate memory for the data to be read
        *data_ptr = new char[sb.st_size + 1];
        // Initialize the allocated memory to zero
        memset(*data_ptr, 0, sb.st_size + 1);
        // Variable to hold the number of bytes read
        ssize_t bytesRead = 0;
        // Read the file using pread64 system call
        if ((bytesRead = pread64(handle->n_handle, *data_ptr, sb.st_size, 0)) < 0)
        {
            // Set the error flag in the handle
            handle->errored = true;
            // Print system error message
            losPrintLastSystemError();
            // Return error status
            return LOS_ERROR_MALFORMED_DATA;
        }
        // Set the number of bytes read
        *bytes_read = bytesRead;
        if (bytesRead != sb.st_size)
            return LOS_ERROR_MALFORMED_DATA;
    }

    // Return success status
    return LOS_SUCCESS;
}

// Writes data to a file.
losResult losWriteFile(_in_ losFileHandle handle, _in_ const void *data_ptr, _in_ const size_t data_size)
{
    // Check if the handle is in an error state
    if (handle->errored || handle->dll_mode)
        return LOS_ERROR_MALFORMED_DATA;
    // Check if the file is in unicode mode.
    if (handle->unicode_mode)
    {
        // File is in unicode mode.
        wchar_t *dptr = static_cast<wchar_t *>(const_cast<void *>(data_ptr));
        // Convert the data to a UTF-8 string.
        std::string buffer = Converter("WCHAR_T", "UTF-8").convert(reinterpret_cast<char *>(dptr), data_size);
        // Write the UTF-8 string to the file using pwrite64.
        if (pwrite64(handle->n_handle, buffer.c_str(), buffer.size(), 0) < 0)
        {
            // Set the error flag in the handle
            handle->errored = true;
            // Print system error message
            losPrintLastSystemError();
            // Return error status
            return LOS_ERROR_MALFORMED_DATA;
        }
    }
    else
    {
        // File is in binary mode.
        // Write the data to the file using pwrite64.
        if (pwrite64(handle->n_handle, data_ptr, data_size, 0) < 0)
        {
            // Set the error flag in the handle
            handle->errored = true;
            // Print system error message
            losPrintLastSystemError();
            // Return error status
            return LOS_ERROR_MALFORMED_DATA;
        }
    }
    // Writing succeeded, return a success code.
    return LOS_SUCCESS;
}

void *losGetFuncAddress(_in_ const losFileHandle handle, _in_ const char *name)
{
    if (handle->errored || !handle->dll_mode)
        return nullptr;
    return dlsym(handle->lib_handle, name);
}
void losUnicodeToBytes(_in_ const wchar_t *src, _out_ char **dest)
{
    // Convert the data to a UTF-8 string.
    std::string data =
        Converter("WCHAR_T", "UTF-8")
            .convert(reinterpret_cast<char *>(const_cast<wchar_t *>(src)), std::char_traits<wchar_t>::length(src));
    // Allocate memory for the data to be read
    *dest = new char[data.size() + 1]; // Add 1 for the null terminator
    // Initialize the allocated memory to zero
    memset(*dest, 0, (data.size() + 1) * sizeof(char));
    // Multiply by sizeof(wchar_t) to get size in bytes
    // Copy the wide string data including the null terminator
    memcpy(*dest, data.c_str(), (data.size() + 1) * sizeof(char));
}

void losBytesToUnicode(_in_ const char *src, _out_ wchar_t **dest)
{
    std::wstring w_data = u8convert(src);
    // Allocate memory for the data to be read
    *dest = new wchar_t[w_data.size() + 1]; // Add 1 for the null terminator
    // Initialize the allocated memory to zero
    memset(*dest, 0, (w_data.size() + 1) * sizeof(wchar_t));
    // Multiply by sizeof(wchar_t) to get size in bytes
    // Copy the wide string data including the null terminator
    memcpy(*dest, w_data.c_str(), (w_data.size() + 1) * sizeof(wchar_t));
}
