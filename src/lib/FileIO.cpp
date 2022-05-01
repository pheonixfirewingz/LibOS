#include "IFileIO.h"
#include <Components/FileIO.h>
#include <vector>
#include <string>
#include <sstream>
#if __has_include(<cpuid.h>)
#   include "Linux/FileIO.hpp"
#endif
#if __has_include(<windows.h>)
#    include "Windows/FileIO.hpp"
#endif

const losResult losOpenFile(losFileHandle *handle, const losFileOpenInfo &info)
{
    if (!(*handle))
        return LOS_ERROR_HANDLE_IN_USE;

    *handle = new losFileHandle_T();
#if __has_include(<windows.h>)
    return winOpenFile(handle, info);
#endif
#if __has_include(<cpuid.h>)
    return linuxOpenFile(handle, info);
#endif
}

const losResult losReadFile(losFileHandle handle,void ** data_ptr, size data_size)
{
#if __has_include(<windows.h>)
    return winReadFile(handle, data_ptr, data_size);
#endif
#if __has_include(<cpuid.h>)
    return linuxReadFile(handle, data_ptr, data_size);
#endif
}

const losResult losWriteFile(losFileHandle handle,const void *data_ptr,const size data_size)
{
#if __has_include(<windows.h>)
    return winWriteFile(handle, std::move(data_ptr), data_size);
#endif
#if __has_include(<cpuid.h>)
    return linuxWriteFile(handle, data_ptr, data_size);
#endif
}

const losResult losCloseFile(losFileHandle handle)
{
#if __has_include(<windows.h>)
    return winCloseFile(handle);
#endif
#if __has_include(<cpuid.h>)
    return linuxCloseFile(handle);
#endif
}

std::vector<std::string> split(std::string s, const char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        if (token.find('\\') != std::string::npos)
        {
            auto path_tokens = split(token.c_str(), '\\');
            for (auto t : path_tokens)
                tokens.push_back(t);
        }
        else
            tokens.push_back(token);
    }
    return tokens;
}

std::string getCorrectPath(const char* path)
{
    std::string ret_path;
    auto path_tokens = split(std::move(path), '/');
    for (auto &tokens : path_tokens)
    {
        if (tokens.starts_with('$'))
        {
            std::string command;
            {
                std::string processed_1 = tokens.substr(2);
                command = processed_1.substr(0, processed_1.size() - 1);
            }
            if (command == "binary_base")
            {
                std::string path_os;
#if __has_include(<windows.h>)
                path_os = std::move(winGetCurrentPath());
#endif
#if __has_include(<cpuid.h>)
                path_os = std::move(linuxGetCurrentPath());
#endif
                auto sun_tuk = split(std::move(path_os), '/');
#if __has_include(<windows.h>)
                for (size i = 0; i < sun_tuk.size() - 1; i++)
#endif
#if __has_include(<cpuid.h>)
                for (size i = 0; i < sun_tuk.size(); i++)
#endif
                    ret_path += (sun_tuk[i] += "/");
            }
        }
        else
            ret_path += (tokens += "/");
    }
    return ret_path.substr(0, ret_path.size() - 1);
}