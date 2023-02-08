// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Defines.h>

#ifdef ON_LINUX
void libOSInit()
{
}

void libOSCleanUp()
{
}
#endif
#ifdef ON_UWP
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
#ifdef ON_WINDOWS
#    include <combaseapi.h>
#    include <comdef.h>
void libOSInit()
{
    HRESULT res = CoInitializeEx(nullptr, COINIT_SPEED_OVER_MEMORY | COINIT_APARTMENTTHREADED);
    if (res != S_OK && res != S_FALSE)
    {
        _com_error err(res);
        LPCTSTR errMsg = err.ErrorMessage();
        printf("LibOS: system error - %s\n", errMsg);
        return;
    }
}

void libOSCleanUp()
{
    CoUninitialize();
}
#endif