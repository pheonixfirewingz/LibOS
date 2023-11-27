// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/NetIO.h>
#include <vector>

extern uint32_t platformNtohl(uint32_t);
extern int32_t platformNtohl(int32_t);
extern uint32_t platformHtonl(uint32_t);
extern int32_t platformHtonl(int32_t);

uint32_t *losNetworkBytesToSystemBytes(const uint32_t *data, const size_t size)
{
    std::vector<uint32_t> bytes;
    bytes.reserve(size);
    for (size_t i = 0; i < size; i++)
        bytes.emplace_back(platformNtohl(data[i]));
    return std::move(bytes.data());
}

uint32_t *losSystemBytesToNetworkBytes(const uint32_t *data, const size_t size)
{
    std::vector<uint32_t> bytes;
    bytes.reserve(size);
    for (size_t i = 0; i < size; i++)
        bytes.emplace_back(platformHtonl(data[i]));
    return std::move(bytes.data());
}

int32_t *losNetworkBytesToSystemBytesSigned(const int32_t *data, const size_t size)
{
    std::vector<int32_t> bytes;
    bytes.reserve(size);
    for (size_t i = 0; i < size; i++)
        bytes.emplace_back(platformNtohl(data[i]));
    return std::move(bytes.data());
}

int32_t *losSystemBytesToNetworkBytesSigned(const int32_t *data, const size_t size)
{
    std::vector<int32_t> bytes;
    bytes.reserve(size);
    for (size_t i = 0; i < size; i++)
        bytes.emplace_back(platformHtonl(data[i]));
    return std::move(bytes.data());
}