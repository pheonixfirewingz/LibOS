#include <libos/FileIO.h>
#include <libos/extensions/Config.h>
#include <string>


losResult loseGetConfig(_out_ losFileHandle *handle, _in_ const char* program_name)
{
    return loseGetNamedConfig(handle,program_name,program_name);
}

losResult loseGetNamedConfig(_out_ losFileHandle *handle, _in_ const char* program_name, _in_ const char * file_name)
{
    std::string path = ((((std::string("$[home]/.config/") + program_name) + "/") + file_name) + ".config");
    losFileOpenInfo handle_info;
    handle_info.path = path.c_str();
    handle_info.path_size = path.size();
    if (losDoseFileExist(path.c_str()) != LOS_SUCCESS)
        handle_info.fileBits = LOS_FILE_BIT_CREATE | LOS_FILE_BIT_READ | LOS_FILE_BIT_WRITE;
    else
        handle_info.fileBits = LOS_FILE_BIT_READ | LOS_FILE_BIT_WRITE;
    return losOpenFile(handle, handle_info);
}