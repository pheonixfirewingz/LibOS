// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <libos/FileIO.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// This struct represents a handle to a file in the LOS filesystem.
struct losFileHandle_T
{
    int32_t n_handle;               // The file descriptor of the file.
    std::string path;               // The absolute path to the file.
    bool flip_endianness = false;   // Whether the file's endianness should be flipped when reading/writing.
    bool unicode_mode = false;      // Whether the file is in Unicode mode.
    bool close_after_done = false;  // Whether the file should be closed after the next read/write operation.
    bool errored = false;           // Whether an error occurred while operating on the file.
};

// A utility function to get the correct path of the file based on platform and asset path
extern std::string getCorrectPath(const std::string);

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

// Returns true if the specified file exists, false otherwise.
losResult losDoseFileExist(const char *path)
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

losResult losOpenFile(losFileHandle *handle, const losFileOpenInfo info)
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

    // Open the file with the specified flags and file mode, and store the file descriptor in n_handle member of
    // losFileHandle object
    if (((*handle)->n_handle = open64((*handle)->path.c_str(), file_flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
    {
        (*handle)->errored = true;
        std::cerr << "system error: " << std::strerror(errno) << std::endl;
        std::cerr << "path: " << (*handle)->path << std::endl;
        return LOS_ERROR_COULD_NOT_INIT;
    }

    // Return success status
    return LOS_SUCCESS;
}

//Closes the file associated with the given handle and releases the resources.
losResult losCloseFile(losFileHandle handle)
{
    if (handle->close_after_done) // If the file should be deleted
        remove(handle->path.c_str()); // Delete the file

    close(handle->n_handle); // Close the file handle
    handle = NULL; // Free the memory used by the handle
    return LOS_SUCCESS; // Return success
}


// Reads data from a file.
losResult losReadFile(losFileHandle handle, void **data_ptr, data_size_t *bytes_read)
{
    // Check if the handle is in an error state
    if (handle->errored)
        return LOS_ERROR_MALFORMED_DATA;
    // Get file information using fstat system call
    struct stat sb
    {
    };
    if (fstat(handle->n_handle, &sb) < 0)
    {
        // Print system error message
        std::cerr << "system error: " << std::strerror(errno) << std::endl;
        // Close the file
        losCloseFile(handle);
        // Return error status
        return LOS_ERROR_MALFORMED_DATA;
    }
    if (!handle->unicode_mode)
    {
        // Allocate memory for the data to be read
        *data_ptr = malloc(sb.st_size);
        // Initialize the allocated memory to zero
        memset(*data_ptr, 0, sb.st_size);
        // Variable to hold the number of bytes read
        sdata_size_t bytesRead = 0;
        // Read the file using pread64 system call
        if ((bytesRead = pread64(handle->n_handle, *data_ptr, sb.st_size, 0)) < 0)
        {
            // Set the error flag in the handle
            handle->errored = true;
            // Print system error message
            std::cerr << "system error: " << std::strerror(errno) << std::endl;
            // Close the file
            losCloseFile(handle);
            // Return error status
            return LOS_ERROR_MALFORMED_DATA;
        }
        // Set the number of bytes read
        *bytes_read = bytesRead;
    }
    else
    {
        // Allocate memory for the data to be read
        *data_ptr = malloc(sb.st_size / sizeof(char8_t));
        // Initialize the allocated memory to zero
        memset(*data_ptr, 0, sb.st_size / sizeof(char8_t));
        // Create a u8string to store the file contents
        std::u8string str;
        // Resize the u8string to hold the file contents
        str.resize(sb.st_size / sizeof(char8_t));
        // Variable to hold the number of bytes read
        sdata_size_t bytesRead = 0;
        // Read the file using pread64 system call
        if ((bytesRead = pread64(handle->n_handle, str.data(), sb.st_size, 0)) < 0)
        {
            // Set the error flag in the handle
            handle->errored = true;
            // Print system error message
            perror("system error");
            // Close the file
            losCloseFile(handle);
            // Return error status
            return LOS_ERROR_MALFORMED_DATA;
        }
        // Copy the contents of the u8string to the allocated memory
        memcpy(*data_ptr, str.c_str(), str.size());
        // Set the number of bytes read
        *bytes_read = bytesRead / sizeof(char8_t);
    }
    // Return success status
    return LOS_SUCCESS;
}

// Writes data to a file.
losResult losWriteFile(losFileHandle handle, const void *data, const data_size_t data_size)
{
    // Check if the file handle has already encountered an error.
    if (handle->errored)
        return LOS_ERROR_MALFORMED_DATA;
    // Check if the file is in unicode mode.
    if (!handle->unicode_mode)
    {
        // File is in binary mode.
        // Write the data to the file using pwrite64.
        if (pwrite64(handle->n_handle, data, data_size, 0) < 0)
        {
            // pwrite64 failed, set the errored flag, print an error message, close the file and return an error code.
            handle->errored = true;
            std::cerr << "system error: " << std::strerror(errno) << std::endl;
            losCloseFile(handle);
            return LOS_ERROR_MALFORMED_DATA;
        }
    }
    else
    {
        // File is in unicode mode.
        // Convert the data to a UTF-8 string.
        std::u8string buffer((const char8_t *)data, 0, data_size);
        // Write the UTF-8 string to the file using pwrite64.
        if (pwrite64(handle->n_handle, buffer.c_str(), buffer.length() * sizeof(char8_t), 0) < 0)
        {
            // pwrite64 failed, set the errored flag, print an error message, close the file and return an error code.
            handle->errored = true;
            std::cerr << "system error: " << std::strerror(errno) << std::endl;
            losCloseFile(handle);
            return LOS_ERROR_MALFORMED_DATA;
        }
    }
    // Writing succeeded, return a success code.
    return LOS_SUCCESS;
}