#pragma once
// this is (DRAFT 1) of the refractal api
#include "Components/Defines.h"
#include "Components/Window.h"

DEFINE_HANDLE(refHandle)
DEFINE_HANDLE(refImage)
DEFINE_HANDLE(refDataBuffer)
DEFINE_HANDLE(refFrameBuffer)
DEFINE_HANDLE(refCommandBuffer)
DEFINE_HANDLE(refhaderProgram)

typedef struct refCreateShaderProgramInfo
{
    std::string shaderLayout;
    std::string vertexShader;
    std::string fragmentShader;
};

typedef enum refDataBufferType
{
    REF_TYPE_VERTEX,
    REF_TYPE_INDEX,
};

typedef struct refCreateDataBuffer
{
    refDataBufferType type;
    void * data;
    size data_size;
};

losResult refCreaterefractalContext(refHandle *,losWindow);
losResult refDestoryrefractalContext(refHandle);

losResult refCreateImage(refImage *);
losResult refCopyDataToImage(refImage,void*,size);
losResult refDestoryImage(refImage);

losResult refCreateDataBuffer(refDataBuffer *, refCreateDataBuffer &);
losResult refCopyDataToDataBuffer(refDataBuffer, void *, size);
losResult refDestoryDataBuffer(refDataBuffer);

losResult refCreateFrameBuffer(refFrameBuffer *);
losResult refDestoryFrameBuffer(refFrameBuffer);

losResult refCreateCommandBuffer(refCommandBuffer *);
losResult refDestoryCommandBuffer(refCommandBuffer);

losResult refCreateShaderProgram(refhaderProgram *, refCreateShaderProgramInfo &);
losResult refDestoryShaderProgram(refhaderProgram);

losResult refBeginFrame(refCommandBuffer);
losResult refBindFrameBuffer(refCommandBuffer, refFrameBuffer);
losResult refBindVertexBuffer(refCommandBuffer, refDataBuffer);
losResult refBindIndexBuffer(refCommandBuffer, refDataBuffer);
losResult refBindShaderProgram(refCommandBuffer, refhaderProgram);
losResult refDrawBuffer(refCommandBuffer,bool,uint32,uint32,uint32,uint32);
losResult refEndFrame(refCommandBuffer);
losResult refExecuteFrame(refCommandBuffer);

losResult refSwapbuffers(losWindow);