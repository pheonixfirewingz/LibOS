/*!
 * \file Config.h
 * \author Luke Shore <lukethemodded@gmail.com>
 * \version 1.0
 * \date 06/06/2023
 * \brief this extention allows the lib user to create and use configs without the hasl of knowing platform conventions
 */
#pragma once
#include "../FileIO.h"
/*!
 * \param[out] handle
 * \param[in] program_name
 * \return losResult
 * \brief loseGetConfig is to create the library object that is attached to the connfig file it also checks if the file is genrated if not make it
 */
EXPORT_DLL losResult loseGetConfig(_out_ losFileHandle *handle, _in_ const char* program_name);
/*!
 * \param[out] handle
 * \param[in] program_name 
 * \param[in] file_name
 * \return losResult
 * \brief loseGetNamedConfig is to create the library object that is attached to the connfig file it also checks if the file is genrated if not make it
 */
EXPORT_DLL losResult loseGetNamedConfig(_out_ losFileHandle *handle, _in_ const char* program_name, _in_ const char * file_name);
