// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/FileIO.h>
#include <string>
#include <vector>

std::string asset_path = "NOT_SET";
extern std::vector<std::string> platformSplit(std::string path) noexcept;
extern std::string platformGetCurrentPath();

losResult losSetAssetPath(const char *path)
{
    asset_path = path;
    return LOS_SUCCESS;
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
#if defined(ON_LINUX)
                    for (data_size_t i = 0; i < sun_tuk.size(); i++)
                        ret_path += (sun_tuk[i] += '/');
#else
                    for (data_size_t i = 0; i < sun_tuk.size() - 1; i++)
                        ret_path += (sun_tuk[i] += '/');
#endif
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
#ifdef ON_LINUX
        if (!ret_path.starts_with('/'))
        ret_path = (std::string() += '/') += ret_path;
#endif
    return ret_path;
}