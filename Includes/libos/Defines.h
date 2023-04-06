/*!
 * \file Defines.h
 * \author Luke Shore <lukethemodded@gmail.com>
 * \version 1.1
 * \date 05/6/2022
 * \brief defines hold function & macros for the parts of the library to work
 */
#pragma once
/*! \brief on windows some extra syntax is required for the dll to work properly so that is dose not affect the
 * platforms we check */
#if defined(ON_WINDOWS) || defined(ON_UWP) || defined(WIN32) || defined(_WIN64)
#    if __cplusplus
#        define EXPORT_DLL extern "C" __declspec(dllexport)
#    else
#        define EXPORT_DLL __declspec(dllexport)
#    endif
#    define EXPORT_CPP_DLL __declspec(dllexport)
#    define _in_ _In_
#    define _out_ _Out_
#else
#    if __cplusplus
#        define EXPORT_DLL extern "C"
#    else
#        define EXPORT_DLL
#    endif
#    define EXPORT_CPP_DLL
#    define _in_
#    define _out_
#endif
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
//
// File Created by Luke Shore 05.12.2022 uk standard
/*! Importation of libraries*/
#include <libos/DataType.h>

#define LOS_MAKE_ABI_VERSION(variant, major, minor, patch) \
    ((((uint32_t)(variant)) << 29U) | (((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

#define LOS_ABI_VERSION_1_1 LOS_MAKE_ABI_VERSION(0, 1, 1, 0)
/*!
 * \brief  due to some of the platforms that libOS abstracts on some platforms some !!!IMPORTANT!!! platform specific
 * initialization code for the library it work
 */
EXPORT_DLL void libOSInit();
/*!
 * \brief  due to some of the platforms that libOS abstracts on some platforms some !!!IMPORTANT!!! platform specific
 * clean up code for the library it do close the app safely
 */
EXPORT_DLL void libOSCleanUp();
/*!
 * \brief  due to some functions may not be avaliable between version this is used tp check at runtime
 */
EXPORT_DLL uint32_t libOSABIVersion();

EXPORT_DLL void losUnicodeToBytes(_in_ const wchar_t *src, _out_ char **dest);

EXPORT_DLL void losBytesToUnicode(_in_ const char *src, _out_ wchar_t **dest);