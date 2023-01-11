#include <Cmake.h>
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/NetIO.h>

losResult tellError() noexcept
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

struct losSocket_T
{
    bool isServer = false;
    bool isTCP = false;
};

losResult platformCreateSocket(losSocket *, const losCreateSocketInfo &)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult platformReadSocket(losSocket , void *, data_size_t *)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losSocket platformWaitForClient(losSocket)
{
    return nullptr;
}

losResult platformWriteSocket(losSocket, const void *, const data_size_t)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult platformDestroySocket(losSocket )
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

uint32_t platformNtohl(uint32_t)
{
    return 0;
}

int32_t platformNtohl(int32_t)
{
    return 0;
}

uint32_t platformHtonl(uint32_t)
{
    return 0;
}

int32_t platformHtonl(int32_t)
{
    return 0;
}