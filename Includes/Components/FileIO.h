#pragma once
#include "Defines.h"

DEFINE_HANDLE(losFileHandle)

typedef enum losFileBits
{
	LOS_FILE_BIT_READ		= 0x001,
	LOS_FILE_BIT_WRITE	= 0x002,
	LOS_FILE_BIT_BINARY	= 0x004,
	LOS_FILE_BIT_CREATE	= 0x008,
	LOS_FILE_BIT_DELETE_AFTER_CLOSE = 0x016,
    LOS_FILE_BIT_FLIP_ENDIEN = 0x032,
} losFileBits;

typedef struct losFileOpenInfo
{
    uint8 fileBits;
    char* path;
	size  path_size;

} losFileOpenInfo;

losResult losOpenFile(losFileHandle *, const losFileOpenInfo &);
losResult losReadFile(losFileHandle, void **, size);
losResult losWriteFile(losFileHandle,const void *,const size);
losResult losCloseFile(losFileHandle);