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
// Copyright Luke Shore (c) 2020, 2023
/*! Importation of libraries*/
#include <libos/Defines.h>

EXPORT_DLL void losPrintLastSystemError();
EXPORT_DLL void losPrintInfo(const char* str);
EXPORT_DLL void losPrintDebug(const char *str);
EXPORT_DLL void losPrintError(const char *str);