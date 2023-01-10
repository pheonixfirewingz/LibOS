/*!
 * \file NetIO.h
 * \author Luke Shore <lukethemodded@gmail.com>
 * \version 1.0
 * \date 05/12/2022
 * \brief NETIO is a simple cross-platform API for accessing UDP and TCP sockets
 */
#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
/*! Importation of libraries*/
#include <libos/Defines.h>
/*!
 *  \brief this is the Library object that is used to read and write network data to and from the Native Socket as the
 * user only holds a pointer to the library object that is controlled by the library.
 */
typedef struct losSocket_T *losSocket;
/*!
 * \brief losSocketBit is used for socket object flags
 */
typedef enum losSocketBit : uint8_t
{
    LOS_SOCKET_UDP = 0x001,    //!< set socket to UDP socket flag bit
    LOS_SOCKET_TCP = 0x002,    //!< set socket to TCP socket flag bit
    LOS_SOCKET_APF = 0x004,    //!< this is for automatically port forwarding in the router flag bit (NOT_IMPLEMENTED)
    LOS_SOCKET_SERVER = 0x008, //!< this is to tell the library that the socket is to be used as a server
} losSocketBit;
/*!
 *  \brief losCreateSocketInfo is the socket creation struct we pass this into the creation function for the library to
 * create a socket object
 */
typedef struct losCreateSocketInfo
{
    uint8_t socket_bits; //!< socket (bits/flags) combination tells the library how to handle the socket
    const char *address; //!< this is used to represent the address for where the socket is to bind or connect to based
                         //!< on if is client or server
    data_size_t address_size; //!< size of string of address
    uint16_t port;            //!< this is to tell the library what port to use for the socket
} losCreateSocketInfo;

/*!
 * \brief simple wrapper used to handle UDP data on the server side
 */
typedef struct losUdpData
{
    losSocket client; //!< client socket
    void *data;       //!< data buffer to receive or send
} losUdpData;
/*!
 * \param[out] bytes
 * \param[in] bytes_size
 * \return losResult
 * \brief losNetworkBytesToSystemBytes converts from big-endian to little-endian(for now as we have not got an arm
 * platform to work with)
 */
EXPORT_DLL uint32_t *losNetworkBytesToSystemBytes(const uint32_t *bytes, const data_size_t bytes_size);
/*!
 * \param[out] bytes
 * \param[in] bytes_size
 * \return losResult
 * \brief losSystemBytesToNetworkBytes converts from little-endian to big-endian(for now as we have not got an arm
 * platform to work with)
 */
EXPORT_DLL uint32_t *losSystemBytesToNetworkBytes(const uint32_t *bytes, const data_size_t bytes_size);
/*!
 * \param[out] bytes
 * \param[in] bytes_size
 * \return losResult
 * \brief losNetworkBytesToSystemBytesSigned converts signed from big-endian to little-endian(for now as we have not got
 * an arm platform to work with)
 */
EXPORT_DLL int32_t *losNetworkBytesToSystemBytesSigned(const int32_t *bytes, const data_size_t bytes_size);
/*!
 * \param[out] bytes
 * \param[in] bytes_size
 * \return losResult
 * \brief losSystemBytesToNetworkBytesSigned converts signed from little-endian to big-endian(for now as we have not got
 * an arm platform to work with)
 */
EXPORT_DLL int32_t *losSystemBytesToNetworkBytesSigned(const int32_t *bytes, const data_size_t bytes_size);
/*!
 * \param[out] handle
 * \param[in] socket_info
 * \return losResult
 * \brief losCreateSocket is to create the library object but dose not release the object to the library user
 */
EXPORT_DLL losResult losCreateSocket(losSocket *handle, const losCreateSocketInfo &socket_info);
/*!
 * \param[in] handle
 * \param[out] buffer
 * \param[in] bytes_to_read
 * \return losResult
 * \brief losReadSocket read data from native socket held by the library object (NOTE: for server side udp it is
 * required to use a losUdpData struct to wrap the buffer to receive data correctly)
 */
EXPORT_DLL losResult losReadSocket(const losSocket handle, void *buffer, const data_size_t bytes_to_read);
/*!
 * \param[in] handle
 * \param[out] new_client
 * \return losResult
 * \brief losWaitForClient is for server side TCP connection only we use this to wait for client connections and return
 * a library object of the connected client
 */
EXPORT_DLL losResult losWaitForClient(const losSocket handle, losSocket *new_client);
/*!
 * \param[in] handle
 * \param[in] buffer
 * \param[in] buffer_size
 * \return losResult
 * \brief losWriteSocket write data to native socket held by the library object (NOTE: for server side udp it is
 * required to use a losUdpData struct to wrap the buffer to send data correctly)
 */
EXPORT_DLL losResult losWriteSocket(const losSocket handle, const void *buffer, const data_size_t buffer_size);
/*!
 * \param[in] handle
 * \return losResult
 * \brief losDestroySocket is to close the library object and safely delete it
 */
EXPORT_DLL losResult losDestroySocket(losSocket handle);
