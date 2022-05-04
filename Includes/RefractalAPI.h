#pragma once
// this is (DRAFT 1) of the refractal api
#include "Components/Defines.h"
#include "Components/Window.h"
#include <string>

DEFINE_HANDLE(refHandle)
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

losResult refCreaterefractalContext(refHandle *,losWindow);
losResult refDestoryrefractalContext(refHandle);

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