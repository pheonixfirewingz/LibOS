// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/NetIO.h>
#    pragma comment(lib, "Ws2_32.lib")
#    define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    include <iphlpapi.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>
#    include <stdio.h>

struct losSocket_T
{
    SOCKET connect_socket = INVALID_SOCKET;
    sockaddr_in server_address = {};
    bool isServer = false;
    bool isTCP = false;
};

#define tellError() __tellError(__func__)
inline losResult __tellError(const char *caller) noexcept
{
    int error = WSAGetLastError();
    switch (error)
    {
    case 0x274D: {
        puts("ERROR: No connection could be made because the target machine actively refused it.\n");
        return LOS_NET_IO_CONNECTION_REFUSED;
    }
    default: {
        wchar_t *s = NULL;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&s, 0, NULL);
        printf("NETIO - %s -> ERRORED: %i -> %S.\n", caller, error, s);
        LocalFree(s);
        return LOS_ERROR_COULD_NOT_INIT;
    }
    }
}

const losResult isLoaded(const bool clear)
{
    static WSADATA wsaData;
    static bool wsainit = false;
    if (wsainit)
    {
        if (clear)
        {
            WSACleanup();
            wsainit = false;
        }
        return LOS_SUCCESS;
    }
    else
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            return LOS_ERROR_COULD_NOT_INIT;
        wsainit = true;
    }
    return LOS_SUCCESS;
}

losResult losCreateSocket(losSocket *socket_in, const losCreateSocketInfo &socket_info)
{
    *socket_in = new (std::nothrow) losSocket_T();
    if ((*socket_in) == NULL)
        return LOS_ERROR_COULD_NOT_INIT;
    if (isLoaded(false) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_INIT;

    switch (socket_info.socket_bits)
    {
    case LOS_SOCKET_TCP | LOS_SOCKET_SERVER:
        (*socket_in)->isServer = true;
        [[fallthrough]];
    case LOS_SOCKET_TCP: {
        (*socket_in)->connect_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        (*socket_in)->isTCP = true;
        break;
    }
    case LOS_SOCKET_UDP | LOS_SOCKET_SERVER:
        (*socket_in)->isServer = true;
        [[fallthrough]];
    case LOS_SOCKET_UDP: {
        (*socket_in)->server_address.sin_family = AF_INET;               // AF_INET = IPv4 addresses
        (*socket_in)->server_address.sin_port = htons(socket_info.port); // Little to big endian conversion
        if ((*socket_in)->isServer)
            (*socket_in)->server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        else
            inet_pton(AF_INET, socket_info.address, &(*socket_in)->server_address.sin_addr);
        (*socket_in)->connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        (*socket_in)->isTCP = false;
        break;
    }
    default:
        return LOS_ERROR_INVALID_FLAGS;
    }

    hostent *ip = gethostbyname(socket_info.address);

    if (ip == NULL)
        return LOS_NET_IO_DOMAIN_NOT_FOUND;

    sockaddr_in sockAddr = {0, 0, {}, {}};
    sockAddr.sin_port = htons(socket_info.port);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = (*reinterpret_cast<unsigned long *>(ip->h_addr_list[0]));

    if ((*socket_in)->isServer)
    {
        if ((bind((*socket_in)->connect_socket, (sockaddr *)(&sockAddr), sizeof(sockAddr))) < 0)
            return tellError();
    }

    if ((*socket_in)->isTCP && !(*socket_in)->isServer)
    {
        if (connect((*socket_in)->connect_socket, (sockaddr *)(&sockAddr), sizeof(sockAddr)) < 0)
            return tellError();
    }

    return LOS_SUCCESS;
}

 losResult losWaitForClient(const losSocket server, losSocket *socket_in)
{
    if (isLoaded(false) != LOS_SUCCESS)
        return LOS_ERROR_MALFORMED_DATA;
    if (listen(server->connect_socket, 10) < 0)
        return tellError();
    (*socket_in) = new losSocket_T();
    (*socket_in)->isTCP = true;
    if (((*socket_in)->connect_socket = accept(server->connect_socket, NULL, NULL)) == -1)
        return tellError();
    return LOS_SUCCESS;
}

losResult losReadSocket(const losSocket socket, void *data, const data_size_t size)
{
    if (isLoaded(false) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_INIT;
    int iResult = 0;
    if (socket->isTCP)
        iResult = recv(socket->connect_socket, (char*)data, (int)size, 0);
    else
    {
        int struct_size = sizeof(sockaddr_in);
        if (socket->isServer)
        {
            losUdpData *casted = (losUdpData *)data;
            memset(casted, 0, sizeof(losUdpData));
            casted->client = new (std::nothrow) losSocket_T();
            if (casted->client == NULL)
                return LOS_ERROR_COULD_NOT_INIT;
            casted->client->isTCP = false;
            casted->client->isServer = false;
            casted->data = new char[size];
            iResult = recvfrom(socket->connect_socket, (char *)casted->data, (int)size, 0,
                               (SOCKADDR *)&casted->client->server_address, &struct_size);
        }
        else
        {
            if (!data)
                data = new char[size];
            iResult = recvfrom(socket->connect_socket, (char *)data, (int)size, 0, (SOCKADDR *)&socket->server_address, &struct_size);
        }
    }
    if (iResult < 0)
        return tellError();
    else if (iResult == 0)
        return LOS_NET_IO_CONNECTION_CLOSED_SERVER_END;
    else
        return LOS_SUCCESS;
}

losResult losWriteSocket(const losSocket socket, const void *data, const data_size_t size)
{
    if (isLoaded(false) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_INIT;
    if (socket->isTCP)
    {
#pragma warning(disable : 4267)
        if (send(socket->connect_socket, (const char*)data, size, 0) < 0)
            return tellError();
    }
    else
    {
        int struct_size = sizeof(sockaddr_in);
        if (socket->isServer)
        {
            losUdpData *casted = (losUdpData *)data;
            if (sendto(socket->connect_socket, (const char *)casted->data, size, 0,
                       (sockaddr *)&casted->client->server_address, struct_size) < 0)
                return tellError();
        }
        else
        {
            
            if (sendto(socket->connect_socket, (const char *)data, size, 0, (sockaddr *)&socket->server_address,struct_size) < 0)
#pragma warning(default : 4267)
                return tellError();
        }
    }
    return LOS_SUCCESS;
}

losResult losDestroySocket(losSocket socket)
{
    if (socket->isTCP)
        shutdown(socket->connect_socket, SD_SEND);
    closesocket(socket->connect_socket);
    if (isLoaded(true) != LOS_SUCCESS)
        return LOS_ERROR_COULD_NOT_DESTROY;
    return LOS_SUCCESS;
}

uint32_t platformNtohl(uint32_t in)
{
    return ntohl(in);
}

int32_t platformNtohl(int32_t in)
{
    return ntohl(in);
}

uint32_t platformHtonl(uint32_t in)
{
    return htonl(in);
}

int32_t platformHtonl(int32_t in)
{
    return htonl(in);
}