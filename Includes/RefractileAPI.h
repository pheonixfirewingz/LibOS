#pragma once
// this is (DRAFT 4) of the refractile api
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
    const char* shader_settings;
    const char* vertex_shader;
    const char* fragment_shader;
    //user is giving direct spirv
    bool pre_compiled = false;
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

typedef enum refSampleCount 
{
    REF_SAMPLE_COUNT_1 = 1,
    REF_SAMPLE_COUNT_2 = 2,
    REF_SAMPLE_COUNT_4 = 4,
    REF_SAMPLE_COUNT_8 = 8,
    REF_SAMPLE_COUNT_16 = 10,
    REF_SAMPLE_COUNT_32 = 20,
    REF_SAMPLE_COUNT_64 = 40
} refSampleCount;

typedef struct refCreateGraphicContextInfo
{
    refSampleCount sample_count = REF_SAMPLE_COUNT_1;
    bool has_depth_stencil = false;

} refCreateGraphicContextInfo;

EXPORT_DLL losResult refAppendGraphicsContext(refHandle,losWindow,const refCreateGraphicContextInfo&) noexcept;
EXPORT_DLL losResult refUnAppendGraphicsContext(refHandle) noexcept;

EXPORT_DLL losResult refCreateFrameBuffer(refHandle,refFrameBuffer *);
EXPORT_DLL losResult refDestroyFrameBuffer(refHandle,refFrameBuffer);

EXPORT_DLL losResult refCreateCommandBuffer(refHandle, refCommandBuffer *);
EXPORT_DLL losResult refDestroyCommandBuffer(refHandle, refCommandBuffer);

EXPORT_DLL losResult refCreateShaderProgram(refHandle, refShaderProgram *, refCreateShaderProgramInfo &);
EXPORT_DLL losResult refDestroyShaderProgram(refHandle, refShaderProgram);

EXPORT_DLL losResult refBeginCommands(refHandle, refCommandBuffer);
EXPORT_DLL losResult refCmdBeginDrawing(refHandle, refCommandBuffer, refFrameBuffer,const float32[4]);
EXPORT_DLL losResult refCmdBindVertexBuffer(refHandle, refCommandBuffer, refDataBuffer);
EXPORT_DLL losResult refCmdBindIndexBuffer(refHandle, refCommandBuffer, refDataBuffer);
EXPORT_DLL losResult refCmdBindShaderProgram(refHandle, refCommandBuffer, refShaderProgram);
EXPORT_DLL losResult refCmdDraw(refHandle, refCommandBuffer,uint32,uint32,uint32,uint32);
EXPORT_DLL losResult refCmdDrawIndexed(refHandle, refCommandBuffer,uint32,uint32,uint32,uint32,uint32);
EXPORT_DLL losResult refCmdEndDrawing(refHandle, refCommandBuffer);
EXPORT_DLL losResult refEndCommands(refHandle, refCommandBuffer);
EXPORT_DLL losResult refExecuteCommands(refHandle, refCommandBuffer, bool);
//---------------------------------------------------------------------------------------------------------
// The Audio API
//---------------------------------------------------------------------------------------------------------
// FIXME: add support for choosing device by user interface
// FIXME: add support for choosing device sound level by user interface
DEFINE_HANDLE(refAudioPlayer)
DEFINE_HANDLE(refAudioBuffer)

EXPORT_DLL losResult refAppendAudioContext(refHandle);
EXPORT_DLL losResult refUnAppendAudioContext(refHandle);

EXPORT_DLL losResult refCreateAudioBuffer(refAudioBuffer *, const char *);
EXPORT_DLL losResult refDestroyAudioBuffer(refAudioBuffer);

EXPORT_DLL losResult refPrepPlayer(refAudioPlayer*);

EXPORT_DLL losResult refPlay(refAudioPlayer, refAudioBuffer, float64, float64, float64, uint8);
EXPORT_DLL losResult refPause(refAudioPlayer);
EXPORT_DLL losResult refResume(refAudioPlayer);
EXPORT_DLL losResult refStop(refAudioPlayer);

//---------------------------------------------------------------------------------------------------------