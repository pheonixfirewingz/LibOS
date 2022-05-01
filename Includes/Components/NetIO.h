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
    uint32 port;
} losCreateSocketInfo;

typedef struct losCreateSocketServerListenInfo
{
    losSocket serverSocket;
    losListenCallback callback;
} losCreateSocketServerListenInfo;

void *losNetworkBytesToSystemBytes(const uint32 *,const size);
void *losSystemBytesToNetworkBytes(const uint32 *, const size);

const losResult losCreateSocket(losSocket *, const losCreateSocketInfo &);
const losResult losReadSocket(losSocket, void *, size *);
const losResult losWriteSocket(losSocket, const void *, const size);
const losResult losListenSocket(const losCreateSocketServerListenInfo &);
const losResult losDestorySocket(losSocket);
