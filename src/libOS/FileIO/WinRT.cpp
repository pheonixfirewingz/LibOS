#include <Cmake.h>
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Defines.h>
#include <libos/FileIO.h>
#include <string>
#include <vector>
#include <winrt/Windows.Foundation.h>
extern std::string getCorrectPath(const char *);

 std::string platformGetCurrentPath()
{
     return {};
}

std::vector<std::string> iSplit(std::string in,char delimiter) noexcept
{
    size_t pos = 0;
    std::vector<std::string> ret;
    do
    {
        size_t start = pos + 1;
        pos = in.find(delimiter, start);
        auto sub = in.substr(start, pos - start);
        if (!sub.empty())
            ret.push_back(sub);
    } while (pos != std::string::npos);
    return ret;
}

extern std::vector<std::string> platformSplit(std::string path) noexcept
{
    return {};
}

losResult platformOpenFile(losFileHandle *, const losFileOpenInfo)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult platformCloseFile(losFileHandle)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult platformReadFile(losFileHandle, void **, data_size_t *)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult platformWriteFile(losFileHandle, const void *, const data_size_t)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}