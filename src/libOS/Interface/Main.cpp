// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Defines.h>

#if ON_LINUX
void libOSInit()
{
}

void libOSCleanUp()
{
}
#endif
#if ON_UWP
#    include <winrt/base.h>

void libOSInit()
{
    winrt::init_apartment();
}

void libOSCleanUp()
{
    winrt::uninit_apartment();
}
#endif
#if ON_WINDOWS
#    include <combaseapi.h>
#    include <comdef.h>
#    define SUCCESS(x)                                                 \
        if ((res = x) != S_OK)                                         \
        {                                                              \
            _com_error err(res);                                       \
            LPCTSTR errMsg = err.ErrorMessage();                       \
            printf("LibOS: Audio WASAPI system error - %s\n", errMsg); \
            return;                                                    \
        }

void libOSInit()
{
    HRESULT res;
    SUCCESS(CoInitializeEx(nullptr, COINIT_SPEED_OVER_MEMORY | COINIT_APARTMENTTHREADED))
}

void libOSCleanUp()
{
}
#endif