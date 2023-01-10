// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <extend_std/Macro.h>
#include <libos/FileIO.h>
#include <string>
#include <vector>

std::string asset_path = "NOT_SET";
extern losResult platformOpenFile(losFileHandle *, const losFileOpenInfo);
extern losResult platformReadFile(losFileHandle, void **, data_size_t *);
extern losResult platformWriteFile(losFileHandle, const void *, const data_size_t);
extern losResult platformCloseFile(losFileHandle);
extern std::vector<std::string> platformSplit(std::string path) noexcept;
extern std::string platformGetCurrentPath();

losResult losSetAssetPath(const char *path)
{
    asset_path = path;
    return LOS_SUCCESS;
}

losResult losOpenFile(losFileHandle *handle, const losFileOpenInfo info)
{
    return platformOpenFile(handle, info);
}

losResult losReadFile(losFileHandle handle, void **data, data_size_t *bytes_read)
{
    return platformReadFile(handle, data, bytes_read);
}

losResult losWriteFile(losFileHandle handle, const void *data, const data_size_t size)
{
    return platformWriteFile(handle, data, size);
}

losResult losCloseFile(losFileHandle handle)
{
    return platformCloseFile(handle);
}

std::string getCorrectPath(const char *path)
{
    std::string ret_path;
    std::vector<std::string> path_tokens = platformSplit(path);

    for (auto &tokens : path_tokens)
    {
        if (tokens[0] == '$')
        {
            std::string command;
            {
                std::string processed_1 = tokens.substr(2);
                command = processed_1.substr(0, processed_1.size() - 1);
            }
            if (command == "binary_base")
            {
                auto sun_tuk = platformSplit(platformGetCurrentPath());
                if constexpr (IS_LINUX())
                {
                    for (data_size_t i = 0; i < sun_tuk.size(); i++)
                        ret_path += (sun_tuk[i] += '/');
                }
                else
                {
                    for (data_size_t i = 0; i < sun_tuk.size() - 1; i++)
                        ret_path += (sun_tuk[i] += '/');
                }
            }
            else if (command == "asset_base")
            {
                ret_path += asset_path;
                ret_path += '/';
            }
        }
        else if (tokens.find('.') != std::string::npos)
            ret_path += tokens;
        else
            ret_path += (tokens += '/');
    }
    if constexpr (IS_LINUX())
    {
#if IS_WINDOWS_UWP()
        if (!ret_path._Starts_with("/"))
#else
        if (!ret_path.starts_with('/'))
#endif
            ret_path = (std::string() += '/') += ret_path;
    }
    return ret_path;
}