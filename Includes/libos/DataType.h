/*!
 * \file DataType.h
 * \author Luke Shore <lukethemodded@gmail.com>
 * \version 1.0
 * \date 05/6/2022
 * \brief DataType is where data types that are used in the library and for the library API are defined
 */
#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
/*! Importation of libraries*/
#include <atomic>
#include <stdint.h>
typedef float float32_t;  //!< this data type is used to represent a 32bit floating point number
typedef double float64_t; //!< this data type is used to represent a 64bit floating point number
typedef uint16_t unicode_t; //!< this data type is basically a uint16_t but from better code readably when working with text it will make more sense to use unicode_t
typedef size_t data_size_t;   //!< this data type is used to represent size of some data unsigned version
#if ON_WINDOWS || ON_UWP || WIN32 || _WIN64
typedef signed long long int sdata_size_t; //!< this data type is used to represent size of some data signed version
#else
typedef ssize_t sdata_size_t; //!< this data type is used to represent size of some data signed version
#endif

typedef std::atomic<float32_t> float32_ts; //!< this is men't do be a thread save version of the float32_t
typedef std::atomic<float64_t> float64_ts; //!< this is men't do be a thread save version of the float64_t

typedef std::atomic<uint8_t> uint8_ts; //!< this is men't do be a thread save version of the uint8_t
typedef std::atomic<uint16_t> uint16_ts; //!< this is men't do be a thread save version of the uint16_t
typedef std::atomic<unicode_t> unicode_ts; //!< this is men't do be a thread save version of the unicode_t
typedef std::atomic<uint32_t> uint32_ts; //!< this is men't do be a thread save version of the uint32_t
typedef std::atomic<uint64_t> uint64_ts; //!< this is men't do be a thread save version of the uint64_t
typedef std::atomic<data_size_t> data_size_ts; //!< this is men't do be a thread save version of the data_size_t

typedef std::atomic<int8_t> int8_ts; //!< this is men't do be a thread save version of the int8_t
typedef std::atomic<int16_t> int16_ts; //!< this is men't do be a thread save version of the int16_t
typedef std::atomic<int32_t> int32_ts; //!< this is men't do be a thread save version of the int32_t
typedef std::atomic<int64_t> int64_ts; //!< this is men't do be a thread save version of the int64_t
typedef std::atomic<sdata_size_t> sdata_size_ts; //!< this is men't do be a thread save version of the sdata_size_t
/*!
 * \brief losSize is used to represent a size of a window or position of this mouse */
typedef struct losSize
{
    int64_t length_one = (int64_t)0; //!< length_one is the same as width or x
    int64_t length_two = (int64_t)0; //!< length_one is the same as height or y
} losSize;
/*!
 * \brief losResult enum is used to indicate the result of a LibOS function call that can fail instead of throwing a
 * Exception
 */
typedef enum losResult
{
    LOS_SUCCESS, //!< this is the value returned if nothing goes wrong in the called library function
    LOS_ERROR_MALFORMED_DATA, //!< this is the value returned if something goes wrong with the native api and unexpected data is returned before it is returned to the user
    LOS_ERROR_COULD_NOT_INIT, //!< this is the value returned if something goes wrong with the native api when creating the library Objects
    LOS_ERROR_COULD_NOT_DESTROY, //!< this is the value returned if something goes wrong with the native api when destroying the library Objects
    LOS_ERROR_INVALID_FLAGS, //!< this is the value returned if some of the flags are invalid when creating the library Objects for the library
    LOS_ERROR_FEATURE_NOT_IMPLEMENTED, //!< this is the value returned if the native api has not being implemented yet
    LOS_NET_IO_DOMAIN_NOT_FOUND, //!< this is the value returned if NETIO can not find the ip to the domain for it to try connecting to
    LOS_NET_IO_CONNECTION_REFUSED, //!< this is the value returned if NETIO goes wrong with the native api connecting refused to finalize
    LOS_NET_IO_CONNECTION_CLOSED_SERVER_END, //!< this is the value returned if NETIO in client mode has the server close the connection while it's in use
    LOS_WINDOW_CLOSE //!< this is the value returned if the window api detects that this window has closed or use asked it to close
} losResult;
