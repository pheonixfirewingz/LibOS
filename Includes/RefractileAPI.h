#pragma once
// this is (DRAFT 2) of the refractile api
#include "Components/Defines.h"
#include "Components/Window.h"
#include <string>

DEFINE_HANDLE(refHandle)
losResult refCreaterefractalContext(refHandle *);
losResult refDestoryrefractalContext(refHandle);

// The Graphics API
//---------------------------------------------------------------------------------------------------------
DEFINE_HANDLE(refImage)
DEFINE_HANDLE(refDataBuffer)
DEFINE_HANDLE(refFrameBuffer)
DEFINE_HANDLE(refCommandBuffer)
DEFINE_HANDLE(refShaderProgram)

typedef struct refCreateShaderProgramInfo
{
    std::string shaderLayout;
    std::string vertexShader;
    std::string fragmentShader;
} refCreateShaderProgramInfo;

typedef enum refDataBufferType
{
    REF_TYPE_VERTEX,
    REF_TYPE_INDEX,
} refDataBufferType;

typedef struct refCreateDataBufferInfo
{
    refDataBufferType type;
    void * data;
    size data_size;
} refCreateDataBufferInfo;

losResult refAppendGraphicsContext(refHandle,losWindow);
losResult refUnappendGraphicsContext(refHandle);

losResult refCreateImage(refImage *);
losResult refCopyDataToImage(refImage,void*,size);
losResult refDestoryImage(refImage);

losResult refCreateDataBuffer(refDataBuffer *, refCreateDataBufferInfo &);
losResult refCopyDataToDataBuffer(refDataBuffer, void *, size);
losResult refDestoryDataBuffer(refDataBuffer);

losResult refCreateFrameBuffer(refFrameBuffer *);
losResult refDestoryFrameBuffer(refFrameBuffer);

losResult refCreateCommandBuffer(refCommandBuffer *);
losResult refDestoryCommandBuffer(refCommandBuffer);

losResult refCreateShaderProgram(refShaderProgram *, refCreateShaderProgramInfo &);
losResult refDestoryShaderProgram(refShaderProgram);

losResult refBeginCommands(refCommandBuffer);
losResult refBindFrameBuffer(refCommandBuffer, refFrameBuffer);
losResult refBindVertexBuffer(refCommandBuffer, refDataBuffer);
losResult refBindIndexBuffer(refCommandBuffer, refDataBuffer);
losResult refBindShaderProgram(refCommandBuffer, refShaderProgram);
losResult refDrawBuffer(refCommandBuffer,bool,uint32,uint32,uint32,uint32);
losResult refEndComands(refCommandBuffer);
losResult refExecuteCommands(refCommandBuffer);

losResult refSwapbuffers(losWindow);
//---------------------------------------------------------------------------------------------------------
// The Audio API
//---------------------------------------------------------------------------------------------------------
DEFINE_HANDLE(refAudioDevice)
DEFINE_HANDLE(refAudioBuffer)

typedef enum refAudioBufferType
{
    WAV,
    MP3,
    FLAC,
    OGG,
} refAudioBufferType;

typedef struct refCreateAudioBufferInfo
{
    refAudioBufferType bufferDataType;
    std::string audioFile;
    float64 pitch;
    float64 scale;
} refCreateAudioBufferInfo;

losResult refAppendAudioContext(refHandle);
losResult refUnappendAudioContext(refHandle);

losResult refGetAudioDeviceList(refHandle, refAudioDevice *);
losResult refSetAudioDevice(refHandle, refAudioDevice);
losResult refSetAudioDeviceSoundLevel(refAudioDevice, float64);

losResult refCreateAudioBuffer(refAudioDevice, refAudioBuffer *, refCreateAudioBufferInfo);
losResult refDestoryAudioBuffer(refAudioDevice, refAudioBuffer);

losResult refPlay(refAudioDevice, refAudioBuffer, float64, float64, float64);
losResult refPause(refAudioDevice, refAudioBuffer);
//---------------------------------------------------------------------------------------------------------