#include <Cmake.h>
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/NetIO.h>
#include <vector>

extern losResult platformCreateSocket(losSocket *, const losCreateSocketInfo &);
extern losResult platformReadSocket(const losSocket, void *, const data_size_t);
extern losResult platformWaitForClient(const losSocket, losSocket *);
extern losResult platformWriteSocket(const losSocket, const void *, const data_size_t);
extern losResult platformDestroySocket(losSocket);
extern uint32_t platformNtohl(uint32_t);
extern int32_t platformNtohl(int32_t);
extern uint32_t platformHtonl(uint32_t);
extern int32_t platformHtonl(int32_t);

uint32_t *losNetworkBytesToSystemBytes(const uint32_t *data, const data_size_t size)
{
    std::vector<uint32_t> bytes;
    bytes.reserve(size);
    for (data_size_t i = 0; i < size; i++)
        bytes.emplace_back(platformNtohl(data[i]));
    return std::move(bytes.data());
}

uint32_t *losSystemBytesToNetworkBytes(const uint32_t *data, const data_size_t size)
{
    std::vector<uint32_t> bytes;
    bytes.reserve(size);
    for (data_size_t i = 0; i < size; i++)
        bytes.emplace_back(platformHtonl(data[i]));
    return std::move(bytes.data());
}

int32_t *losNetworkBytesToSystemBytesSigned(const int32_t *data, const data_size_t size)
{
    std::vector<int32_t> bytes;
    bytes.reserve(size);
    for (data_size_t i = 0; i < size; i++)
        bytes.emplace_back(platformNtohl(data[i]));
    return std::move(bytes.data());
}

int32_t *losSystemBytesToNetworkBytesSigned(const int32_t *data, const data_size_t size)
{
    std::vector<int32_t> bytes;
    bytes.reserve(size);
    for (data_size_t i = 0; i < size; i++)
        bytes.emplace_back(platformHtonl(data[i]));
    return std::move(bytes.data());
}

losResult losCreateSocket(losSocket *socket, const losCreateSocketInfo &info)
{
    return platformCreateSocket(socket, info);
}

losResult losReadSocket(const losSocket socket, void *data, const data_size_t size)
{
    return platformReadSocket(socket, data, size);
}

losResult losWaitForClient(const losSocket server, losSocket *client)
{
    return platformWaitForClient(server,client);
}

losResult losWriteSocket(const losSocket socket, const void *data, const data_size_t size)
{
    return platformWriteSocket(socket, data, size);
}

losResult losDestroySocket(losSocket socket)
{
    return platformDestroySocket(socket);
}