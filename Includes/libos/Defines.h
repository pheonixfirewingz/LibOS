/*!
 * \file Defines.h
 * \author Luke Shore <lukethemodded@gmail.com>
 * \version 1.1
 * \date 05/12/2022
 * \brief defines hold function & macros for the parts of the library to work
 */
#pragma once
/*! \brief on windows some extra syntax is required for the dll to work properly so that is dose not affect the
 * platforms we check */
#if defined(ON_WINDOWS) || defined(ON_UWP)
/*! \brief makes sure c++ dose not mangle names and tells MSVC to gen dll .lib bindings */
#    if __cplusplus
#        define EXPORT_DLL extern "C" __declspec(dllexport)
#    else
#        define EXPORT_DLL __declspec(dllexport)
#    endif
/*! \brief MSVC to gen dll .lib bindings */
#    define EXPORT_CPP_DLL __declspec(dllexport)
/*!
 * \brief  _in_ macromatic suger to make headers easier to read also alows MSVC to optimize better
 */
#    define _in_ _In_
/*!
 * \brief  _out_ macromatic suger to make headers easier to read also alows MSVC to optimize better
 */
#    define _out_ _Out_
#else
/*! \brief makes sure c++ dose not mangle names */
#    if __cplusplus
#        define EXPORT_DLL extern "C"
#    else
#        define EXPORT_DLL
#    endif
/*! \brief MSVC to gen dll .lib bindings */
#    define EXPORT_CPP_DLL
/*!
 * \brief  _in_ macromatic suger to make headers easier to read
 */
#    define _in_
/*!
 * \brief  _out_ macromatic suger to make headers easier to read
 */
#    define _out_
#endif
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2022, 2023
//
/*! Importation of libraries*/
#include <libos/DataType.h>
/*!
 * \brief  a quick dirty macro to make lib versions like vulkan
 */
#define LOS_MAKE_ABI_VERSION(variant, major, minor, patch) \
    (((static_cast<uint32_t>(variant)) << 29U) | ((static_cast<uint32_t>(major)) << 22U) | ((static_cast<uint32_t>(minor)) << 12U) | (static_cast<uint32_t>(patch)))
/*! \brief defines that this header set is designed for v1.1 at a minimumum for checks*/
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
/*!
 * \brief  converts Unicode String to an byte array
 */
EXPORT_DLL void losUnicodeToBytes(_in_ const wchar_t *src, _out_ char **dest);
/*!
 * \brief  converts an byte array to Unicode String
 */
EXPORT_DLL void losBytesToUnicode(_in_ const char *src, _out_ wchar_t **dest);
#ifdef ON_ANDROID
EXPORT_DLL int main(int, char **);
#endif