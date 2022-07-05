#pragma once
#include "Headers/uk_co_lunalux_libos_FileIO.h"
#include <stdint.h>
#include <Components/FileIO.h>
#include <string>
#include <stdexcept>

class FileIOHandler
{
    struct FileFrame
    {
        losResult result = LOS_SUCCESS;
        losFileHandle fileHandle = NULL;
        bool free = true;
    } frames[256]; 

    jint getNextFree() const noexcept
    {
        for(jint i = 0; i < 255; i++)
        {
            if(frames[i].free) 
                return i;
        }
        return INT32_MAX;
    }
    public:
    FileIOHandler(const FileIOHandler&) = delete;
    FileIOHandler(FileIOHandler&&) = delete;
    explicit FileIOHandler() {};
    static FileIOHandler& getInstance()
    {
        static FileIOHandler instance;
        return instance;
    }

    jint j_losOpenFile(jint fileBits, const char* path, const size path_size)
    {
        jint id = getNextFree();
        losFileOpenInfo info;
        info.fileBits = fileBits;
        info.path = path;
        info.path_size = path_size;
        frames[id].result = losOpenFile(&frames[id].fileHandle,info);
        frames[id].free = false;
        return id;
    }
    
    void j_losWriteFile(jint file_id,std::string data)
    {
        frames[file_id].result = losWriteFile(frames[file_id].fileHandle,reinterpret_cast<const void*>(data.c_str()),data.size());
    }

    const char* j_losReadFile(jint file_id)
    {
        void* buffer;
        size buffer_size;
        frames[file_id].result = losReadFile(frames[file_id].fileHandle,&buffer,&buffer_size);
        return std::move(static_cast<char*>(buffer));
    }

    void j_losCloseFile(jint file_id)
    {
        frames[file_id].result = losCloseFile(frames[file_id].fileHandle);
        frames[file_id].fileHandle = NULL;
        frames[file_id].free = true;
    }

    losResult j_getCurrentResult(jint file_id) noexcept
    {
        return frames[file_id].result;
    }


    void j_setAssetPath(const char* path)
    {
        if(losSetAssetPath(path) != LOS_SUCCESS) throw std::runtime_error("could not set asset path");
    }
};

