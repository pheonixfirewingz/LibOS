#pragma once
// this is (DRAFT 2) of the refractile api
#include "Components/Defines.h"
#include "Components/Window.h"

DEFINE_HANDLE(refHandle)
EXPORT_DLL losResult refCreateRefractileContext(refHandle *);
EXPORT_DLL losResult refDestroyRefractileContext(refHandle);
//---------------------------------------------------------------------------------------------------------
// The Graphics API
//---------------------------------------------------------------------------------------------------------
DEFINE_HANDLE(refImage)
DEFINE_HANDLE(refDataBuffer)
DEFINE_HANDLE(refFrameBuffer)
DEFINE_HANDLE(refCommandBuffer)
DEFINE_HANDLE(refShaderProgram)

typedef struct refCreateShaderProgramInfo
{
    const char* shaderLayout;
    const char* vertexShader;
    const char* fragmentShader;
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

EXPORT_DLL losResult refAppendGraphicsContext(refHandle,losWindow);
EXPORT_DLL losResult refUnAppendGraphicsContext(refHandle);

EXPORT_DLL losResult refCreateImage(refImage *);
EXPORT_DLL losResult refCopyDataToImage(refImage,void*,size);
EXPORT_DLL losResult refDestroyImage(refImage);

EXPORT_DLL losResult refCreateDataBuffer(refDataBuffer *, refCreateDataBufferInfo &);
EXPORT_DLL losResult refCopyDataToDataBuffer(refDataBuffer, void *, size);
EXPORT_DLL losResult refDestroyDataBuffer(refDataBuffer);

EXPORT_DLL losResult refCreateFrameBuffer(refFrameBuffer *);
EXPORT_DLL losResult refDestroyFrameBuffer(refFrameBuffer);

EXPORT_DLL losResult refCreateCommandBuffer(refHandle, refCommandBuffer *);
EXPORT_DLL losResult refDestroyCommandBuffer(refHandle, refCommandBuffer);

EXPORT_DLL losResult refCreateShaderProgram(refHandle, refShaderProgram *, refCreateShaderProgramInfo &);
EXPORT_DLL losResult refDestroyShaderProgram(refHandle, refShaderProgram);

EXPORT_DLL losResult refBeginCommands(refHandle, refCommandBuffer);
EXPORT_DLL losResult refBindFrameBuffer(refHandle, refCommandBuffer, refFrameBuffer);
EXPORT_DLL losResult refBindVertexBuffer(refHandle, refCommandBuffer, refDataBuffer);
EXPORT_DLL losResult refBindIndexBuffer(refHandle, refCommandBuffer, refDataBuffer);
EXPORT_DLL losResult refBindShaderProgram(refHandle, refCommandBuffer, refShaderProgram);
EXPORT_DLL losResult refDraw(refHandle, refCommandBuffer,uint32,uint32,uint32,uint32,uint32);
EXPORT_DLL losResult refDrawIndexed(refHandle, refCommandBuffer,uint32,uint32,uint32,uint32,uint32);
EXPORT_DLL losResult refEndCommands(refHandle, refCommandBuffer);
EXPORT_DLL losResult refExecuteCommands(refHandle, refCommandBuffer, bool);
//---------------------------------------------------------------------------------------------------------
// The Audio API
//---------------------------------------------------------------------------------------------------------
DEFINE_HANDLE(refAudioDevice)
DEFINE_HANDLE(refAudioPlayer)
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
    char* audioFile;
    float64 pitch;
    float64 scale;
} refCreateAudioBufferInfo;

EXPORT_DLL losResult refAppendAudioContext(refHandle);
EXPORT_DLL losResult refUnAppendAudioContext(refHandle);

EXPORT_DLL losResult refGetAudioDeviceList(refHandle, refAudioDevice *);
EXPORT_DLL losResult refSetAudioDevice(refHandle, refAudioDevice);
EXPORT_DLL losResult refSetAudioDeviceSoundLevel(refAudioDevice, float64);

EXPORT_DLL losResult refCreateAudioBuffer(refAudioDevice, refAudioBuffer *, refCreateAudioBufferInfo);
EXPORT_DLL losResult refDestoryAudioBuffer(refAudioDevice, refAudioBuffer);

EXPORT_DLL losResult refPlay(refAudioDevice,refAudioPlayer*, refAudioBuffer, float64, float64, float64, uint8);
EXPORT_DLL losResult refPause(refAudioDevice,refAudioPlayer);
EXPORT_DLL losResult refResume(refAudioDevice,refAudioPlayer);
EXPORT_DLL losResult refStop(refAudioDevice,refAudioPlayer);

//---------------------------------------------------------------------------------------------------------