#include "INetIO.h"
#include <Components/NetIO.h>
#if __has_include(<windows.h>)
#    include "Windows/NetIO.hpp"
#endif
#if __has_include(<arpa/inet.h>)
#    include "Linux/NetIO.hpp"
#endif

void *losNetworkBytesToSystemBytes(const uint32 *data, const size data_size)
{
#if __has_include(<windows.h>)
    return winNetworkBytesToSystemBytes(data, data_size);
#endif
#if __has_include(<arpa/inet.h>)
    return linuxNetworkBytesToSystemBytes(data, data_size);
#endif
}

void *losSystemBytesToNetworkBytes(const uint32 *data, const size data_size)
{
#if __has_include(<windows.h>)
    return winSystemBytesToNetworkBytes(data, data_size);
#endif
#if __has_include(<arpa/inet.h>)
    return linuxSystemBytesToNetworkBytes(data, data_size);
#endif
}

const losResult losCreateSocket(losSocket *socket, const losCreateSocketInfo &socket_info)
{
#if __has_include(<windows.h>)
    return winCreateSocket(socket, socket_info);
#endif
#if __has_include(<arpa/inet.h>)
    return linuxCreateSocket(socket, socket_info);
#endif
}

const losResult losReadSocket(losSocket socket, void * data, size *data_size)
{
#if __has_include(<windows.h>)
    return winReadSocket(socket, data, data_size);
#endif
#if __has_include(<arpa/inet.h>)
    return linuxReadSocket(socket, data, data_size);
#endif
}

const losResult losWriteSocket(losSocket socket,const void *data, const size data_size)
{
#if __has_include(<windows.h>)
    return winWriteSocket(socket, data, data_size);
#endif
#if __has_include(<arpa/inet.h>)
    return linuxWriteSocket(socket, data, data_size);
#endif
}

const losResult losListenSocket(const losCreateSocketServerListenInfo &listen_info)
{
#if __has_include(<windows.h>)
    return winListenSocket(listen_info);
#endif
#if __has_include(<arpa/inet.h>)
    return linuxListenSocket(listen_info);
#endif
}

const losResult losDestorySocket(losSocket socket)
{
#if __has_include(<windows.h>)
    return winDestorySocket(socket);
#endif
#if __has_include(<arpa/inet.h>)
    return linuxDestorySocket(socket);
#endif
}