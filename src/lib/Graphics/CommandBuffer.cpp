#include <RefractileAPI.h>
#include "../InternalRefractile.h"

losResult refCreateCommandBuffer(refHandle, refCommandBuffer *)
{
    return LOS_SUCCESS;
}

losResult refDestroyCommandBuffer(refHandle, refCommandBuffer)
{
    return LOS_SUCCESS;
}

losResult refBeginCommands(refHandle, refCommandBuffer)
{
    return LOS_SUCCESS;
}

losResult refBindFrameBuffer(refHandle, refCommandBuffer, refFrameBuffer)
{
    return LOS_SUCCESS;
}

losResult refBindVertexBuffer(refHandle, refCommandBuffer, refDataBuffer)
{
    return LOS_SUCCESS;
}

losResult refBindIndexBuffer(refHandle, refCommandBuffer, refDataBuffer)
{
    return LOS_SUCCESS;
}

losResult refBindShaderProgram(refHandle, refCommandBuffer, refShaderProgram)
{
    return LOS_SUCCESS;
}

losResult refDraw(refHandle, refCommandBuffer,uint32,uint32,uint32,uint32,uint32)
{
    return LOS_SUCCESS;
}

losResult refDrawIndexed(refHandle, refCommandBuffer,uint32,uint32,uint32,uint32,uint32)
{
    return LOS_SUCCESS;
}

losResult refEndCommands(refHandle, refCommandBuffer)
{
    return LOS_SUCCESS;
}

losResult refExecuteCommands(refHandle, refCommandBuffer,bool present)
{
    if (present)
    {

    }
    else
    {

    }
    return LOS_SUCCESS;
}