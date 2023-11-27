// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <libos/NetIO.h>
#include <libos/Error.h>

losResult tellError() noexcept
{
    losPrintLastSystemError();
    return LOS_NET_IO_CONNECTION_REFUSED;
}

struct losSocket_T
{
    int32_t connect_socket = 0;
    struct sockaddr_in server_address = {};
    bool isServer = false;
    bool isTCP = false;
    bool failed = false;
};

losResult losCreateSocket(losSocket *socket_in, const losCreateSocketInfo &socket_info)
{
    if (!(*socket_in = new losSocket_T()))
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

    if (ip == nullptr)
        return LOS_NET_IO_DOMAIN_NOT_FOUND;

    sockaddr_in sockAddr = {0, 0, {}, {}};
    sockAddr.sin_port = htons(socket_info.port);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = (*reinterpret_cast<unsigned long *>(ip->h_addr_list[0]));

    if ((*socket_in)->isServer)
    {
        if ((bind((*socket_in)->connect_socket, reinterpret_cast<sockaddr *>(&sockAddr), sizeof(sockAddr))) < 0)
        {
            (*socket_in)->failed = true;
            return tellError();
        }
    }

    if ((*socket_in)->isTCP && !(*socket_in)->isServer)
    {
        if (connect((*socket_in)->connect_socket, reinterpret_cast<sockaddr *>(&sockAddr), sizeof(sockAddr)) < 0)
        {
            (*socket_in)->failed = true;
            return tellError();
        }
    }

    return LOS_SUCCESS;
}

losResult losWaitForClient(const losSocket server, losSocket *socket_in)
{
    if(server->failed)
        return LOS_ERROR_MALFORMED_DATA;
    if (listen(server->connect_socket, 10) < 0)
    {
        (*socket_in)->failed = true;
        return tellError();
    }
    if (!(*socket_in = new losSocket_T()))
        return LOS_ERROR_COULD_NOT_INIT;
    (*socket_in)->isTCP = true;
    if (((*socket_in)->connect_socket = accept(server->connect_socket, nullptr, nullptr)) == -1)
    {
        (*socket_in)->failed = true;
        return tellError();
    }
    return LOS_SUCCESS;
}

losResult losReadSocket(const losSocket socket, void *data, const size_t size)
{
    if (socket->failed)
        return LOS_ERROR_MALFORMED_DATA;
    int iResult = 0;
    if (socket->isTCP)
    {
        if (!data)
            data = new char[size];
        memset(data,0,size);
        iResult = recv(socket->connect_socket, static_cast<char *>(data), static_cast<int>(size), 0);
    }
    else
    {
        socklen_t struct_size = sizeof(sockaddr_in);
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        setsockopt(socket->connect_socket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
        if (socket->isServer)
        {
            auto *casted = static_cast<losUdpData*>(data);
            memset(casted, 0, sizeof(losUdpData));
            casted->client = new losSocket_T();
            if (casted->client == nullptr)
                return LOS_ERROR_COULD_NOT_INIT;
            casted->client->isTCP = false;
            casted->client->isServer = false;
            casted->data = new char[size];
            iResult = recvfrom(socket->connect_socket, static_cast<char *>(casted->data), static_cast<int>(size), 0,
                               reinterpret_cast<sockaddr *>(&casted->client->server_address), &struct_size);
        }
        else
        {
            if (!data)
                data = new char[size];
            memset(data,0,size);
            iResult = recvfrom(socket->connect_socket, static_cast<char *>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr *>(&socket->server_address),
                               &struct_size);
        }
    }
    if (iResult < 0)
        return tellError();
    else if (iResult == 0)
        return LOS_NET_IO_CONNECTION_CLOSED_SERVER_END;
    else
        return LOS_SUCCESS;
}

losResult losWriteSocket(const losSocket socket, const void *data, const size_t size)
{
    if (socket->failed)
        return LOS_ERROR_MALFORMED_DATA;
    if (socket->isTCP)
    {
        if (send(socket->connect_socket, static_cast<const char *>(data), size, 0) < 0)
            return tellError();
    }
    else
    {
        socklen_t struct_size = sizeof(sockaddr_in);
        if (socket->isServer)
        {
            auto *casted = static_cast<const losUdpData*>(data);
            if (sendto(socket->connect_socket, static_cast<const char *>(casted->data), size, 0,
                       reinterpret_cast<sockaddr *>(&casted->client->server_address), struct_size) < 0)
                return tellError();
        }
        else
        {

            if (sendto(socket->connect_socket, static_cast<const char *>(data), size, 0, reinterpret_cast<sockaddr *>(&socket->server_address),
                       struct_size) < 0)
                return tellError();
        }
    }
    return LOS_SUCCESS;
}

losResult losDestroySocket(losSocket socket)
{
    if (socket->isTCP)
        shutdown(socket->connect_socket, SHUT_RDWR);
    close(socket->connect_socket);
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