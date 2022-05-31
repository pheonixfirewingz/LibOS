#include "InternalRefractile.h"
#include <RefractileAPI.h>

losResult refCreateRefractileContext(refHandle *handle)
{
    (*handle) = new refHandle_T();
    return LOS_SUCCESS;
}

losResult refDestoryRefractileContext(refHandle handle)
{
    delete handle;
    return LOS_SUCCESS;
}