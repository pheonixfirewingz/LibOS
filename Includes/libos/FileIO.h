/*!
 * \file FileIO.h
 * \author Luke Shore <lukethemodded@gmail.com>
 * \version 1.2
 * \date 05/6/2022
 * \brief FileIO is a simple cross-platform API for reading and writing files from the filesystem
 */
#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2022, 2023
/*! Importation of libraries*/
#include <libos/Defines.h>
/*!
 *  \brief this is the Library object that is used to read and write files from the filesystem as the user only holds a
 * pointer to the library object that is controlled by the library.
 */
typedef struct losFileHandle_T *losFileHandle;
/*!
 * \brief losFileBits is used for file object flags
 */
typedef enum losFileBits
{
    LOS_FILE_BIT_READ = 0x01,               //!< Read file flag bit
    LOS_FILE_BIT_WRITE = 0x02,              //!< Write file flag bit
    LOS_FILE_BIT_BINARY = 0x04,             //!< Binary aka library handle file flag bit (NOT_IMPLEMENTED)
    LOS_FILE_BIT_UNICODE = 0x08,            //!< Unicode file flag bit
    LOS_FILE_BIT_CREATE = 0x10,             //!< Create file if does not exist flag bit
    LOS_FILE_BIT_DELETE_AFTER_CLOSE = 0x20, //!< Tells the library to delete the file after using it flag bit
    LOS_FILE_BIT_FLIP_ENDIAN = 0x40,        //!< Flip the file's read bits from the original flag bit (NOT_IMPLEMENTED)
} losFileBits;
/*!
 *  \brief losFileOpenInfo is the file creation struct we pass this into the creation function for the library to create
 * a file object
 */
typedef struct losFileOpenInfo
{
    uint8_t fileBits = 0; //!< file (bits/flags) combination tells the library how to lock the file down to act like a
                          //!< sanity check
    const char *path; //!< path to the file $[binary_base] and $[asset_base] can be used as relative path for internal
                      //!< program file path handling
    size_t path_size = 0; //!< size of the path to the file

} losFileOpenInfo;
/*!
 * \param[in] path
 * \return losResult
 * \brief losSetAssetPath can be used to set the full data path for $[asset_base]
 */
EXPORT_DLL losResult losSetAssetPath(_in_ const char *path);
/*!
 * \param[in] path
 * \return losResult
 * \brief losDoseFileExist can be used to check if the file exists will return LOS_SUCCESS if the file exists else it will return LOS_ERROR_MALFORMED_DATA
 */
EXPORT_DLL losResult losDoseFileExist(_in_ const char *path);
/*!
 * \param[out] handle
 * \param[in] file_info
 * \return losResult
 * \brief losOpenFile is to create the library object but dose not release the object to the library user
 */
EXPORT_DLL losResult losOpenFile(_out_ losFileHandle *handle, _in_ const losFileOpenInfo file_info);
/*!
 * \param[in] handle
 * \param[out] buffer
 * \param[out] bytes_read
 * \return losResult
 * \brief losReadFile read data to native file held by the library object when in unicode mode it will return wchar_t data
 * encoding from the read
 */
EXPORT_DLL losResult losReadFile(_in_ losFileHandle handle, _out_ void **buffer, _out_ size_t *bytes_read);
/*!
 * \param[in] handle
 * \param[in] buffer
 * \param[in] buffer_size
 * \return losResult
 * \brief losWriteFile write data to native file held by the library object when in unicode mode it expects wchar_t data
 * encoding to write
 */
EXPORT_DLL losResult losWriteFile(_in_ const losFileHandle handle, _in_ const void *buffer,
                                  _in_ const size_t buffer_size);
/*!
 * \param[in] handle
 * \param[in] name
 * \return address
 * \brief losGetFuncAddress extracts the function memory address loaded when the LOS_FILE_BIT_BINARY is set
 */
EXPORT_DLL void *losGetFuncAddress(_in_ const losFileHandle handle, _in_ const char *name);
/*!
 * \param[in] handle
 * \return losResult
 * \brief losCloseFile is to close the library object and safely delete it
 */
EXPORT_DLL losResult losCloseFile(_in_ losFileHandle handle);
