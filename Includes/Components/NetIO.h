#pragma once
#include "Defines.h"
DEFINE_HANDLE(losSocket)

typedef losResult (*losListenCallback)(losSocket);

typedef enum losSocketBit : uint8
{
	LOS_SOCKET_UDP = 0x001,
	LOS_SOCKET_TCP = 0x002,
	//this is auto port forward
	LOS_SOCKET_APF = 0x004,
    LOS_SOCKET_SERVER = 0x008,
} losSocketBit;

typedef struct losCreateSocketInfo
{
    uint8 socketBits;
    char* address;
    size address_size;
    uint16 port;
} losCreateSocketInfo;

typedef struct losCreateSocketServerListenInfo
{
    losSocket serverSocket;
    losListenCallback callback;
} losCreateSocketServerListenInfo;

EXPORT_DLL void *losNetworkBytesToSystemBytes(const uint32 *,const size);
EXPORT_DLL void *losSystemBytesToNetworkBytes(const uint32 *, const size);

EXPORT_DLL losResult losCreateSocket(losSocket *, const losCreateSocketInfo &);
EXPORT_DLL losResult losReadSocket(losSocket, void *, size *);
EXPORT_DLL losResult losWriteSocket(losSocket, const void *, const size);
EXPORT_DLL losResult losListenSocket(losCreateSocketServerListenInfo &);
EXPORT_DLL losResult losDestorySocket(losSocket);
