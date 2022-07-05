#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include "Components/Defines.h"
#include <Components/NetIO.h>
#include <arpa/inet.h>
#include <cstdio>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

struct losSocket_T
{
    int32 ConnectSocket = 0;
    struct sockaddr_in server_address;
    int server_address_size = 0;
    bool isServer = false;
    bool isTCP = false;
};

void *losNetworkBytesToSystemBytes(const uint32 *data, const size data_size)
{
    std::vector<uint32> bytes;
    bytes.reserve(data_size);
    for (size i = 0; i < data_size; i++)
        bytes.emplace_back(ntohl(data[i]));
    return std::move(bytes.data());
}

void *losSystemBytesToNetworkBytes(const uint32 *data, const size data_size)
{
    std::vector<uint32> bytes;
    bytes.reserve(data_size);
    for (size i = 0; i < data_size; i++)
        bytes.emplace_back(htonl(data[i]));
    return std::move(bytes.data());
}

losResult tellError()
{
    perror("system error");
    return LOS_SUCCESS;
}

losResult losCreateSocket(losSocket *socket_in, const losCreateSocketInfo &socket_info)
{
    if (!(*socket_in))
        return LOS_ERROR_HANDLE_IN_USE;

    *socket_in = new losSocket_T();
    switch (socket_info.socketBits)
    {
    case LOS_SOCKET_TCP | LOS_SOCKET_SERVER:
        (*socket_in)->isServer = true;
        [[fallthrough]];
    case LOS_SOCKET_TCP: {
        (*socket_in)->ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        (*socket_in)->isTCP = true;
        break;
    }
    case LOS_SOCKET_UDP | LOS_SOCKET_SERVER:
        (*socket_in)->isServer = true;
        [[fallthrough]];
    case LOS_SOCKET_UDP: {
        (*socket_in)->server_address.sin_family = AF_INET;               // AF_INET = IPv4 addresses
        (*socket_in)->server_address.sin_port = htons(socket_info.port); // Little to big endian conversion
        inet_pton(AF_INET, socket_info.address,
                  &(*socket_in)->server_address.sin_addr); // Convert from string to byte array
        (*socket_in)->server_address_size = sizeof((*socket_in)->server_address);
        (*socket_in)->ConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        (*socket_in)->isTCP = false;
        break;
    }
    default:
        return LOS_ERROR_INVALID_FLAGS;
    }

    if ((*socket_in)->isServer)
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;

    hostent *ip = gethostbyname(socket_info.address);

    if (ip == NULL)
        return LOS_NET_IO_DOMAIN_NOT_FOUND;

    if ((*socket_in)->isTCP)
    {
        sockaddr_in sockAddr = {0, 0, {}, {}};
        sockAddr.sin_port = htons(socket_info.port);
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_addr.s_addr = (*reinterpret_cast<unsigned long *>(ip->h_addr_list[0]));

        if (connect((*socket_in)->ConnectSocket, (sockaddr *)(&sockAddr), sizeof(sockAddr)) < 0)
            return tellError();
    }

    return LOS_SUCCESS;
}

losResult losReadSocket(losSocket socket, void *data, size *data_size)
{
    int iResult = 0;
    if (socket->isTCP)
        iResult = recv(socket->ConnectSocket, (char *)data, *data_size, 0);
    else
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;

    if (iResult < 0)
        return tellError();
    else if (iResult == 0)
        return LOS_NET_IO_CONNECTION_CLOSED_SERVER_END;
    else
        return LOS_SUCCESS;
}

losResult losWriteSocket(losSocket socket, const void *data, const size data_size)
{
    if (socket->isTCP)
    {
        if (send(socket->ConnectSocket, (const char *)data, data_size, 0) < 0)
            return tellError();
    }
    else
    {
        if (sendto(socket->ConnectSocket, (const char *)data, data_size, 0, (struct sockaddr *)&socket->server_address,
                   socket->server_address_size) < 0)
            return tellError();
    }
    return LOS_SUCCESS;
}

losResult losListenSocket(const losCreateSocketServerListenInfo &)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losDestorySocket(losSocket socket)
{
    if (shutdown(socket->ConnectSocket, SHUT_RDWR) < 0)
    {
        close(socket->ConnectSocket);
        return tellError();
    }
    close(socket->ConnectSocket);
    return LOS_SUCCESS;
}
#endif