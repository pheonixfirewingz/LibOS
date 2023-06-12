/*!
 * \file Error.h
 * \author Luke Shore <lukethemodded@gmail.com>
 * \version 1.0
 * \date 04/4/2023
 * \brief for printing and system errors
 */
#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2023
/*! Importation of libraries*/
#include <libos/Defines.h>
/*!
 * \brief losPrintLastSystemError get and print last native system error
 */
EXPORT_DLL void losPrintLastSystemError();
/*!
 * \param[in] msg
 * \brief losPrintInfo print a normal message
 */
EXPORT_DLL void losPrintInfo(const char* msg);
/*!
 * \param[in] msg
 * \brief losPrintDebug print a debug msg
 */
EXPORT_DLL void losPrintDebug(const char *msg);
/*!
 * \param[in] msg
 * \brief losPrintError print a program error
 */
EXPORT_DLL void losPrintError(const char *msg);