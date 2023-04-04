// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Error.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

LPSTR GetFormattedMessage(LPCVOID pMessage)
{
    LPSTR pBuffer = NULL;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, pMessage, 0, 0, (LPSTR)&pBuffer, 0,
                  NULL);
    return pBuffer;
}

void losPrintLastSystemError()
{
#if IS_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4312)
#endif
    losPrintError((std::string("-> SYSTEM ERROR: ") += GetFormattedMessage(reinterpret_cast<LPCVOID>(GetLastError()))).c_str());
#if IS_MSVC
#    pragma warning(pop)
#endif
}

void losPrintInfo(const char *str)
{
    DWORD dwWritten;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string out("PROGRAM INFO: ");
    out += str;
    out += "\n";
    WriteConsole(hConsole, out.c_str(), DWORD(out.size()), &dwWritten, nullptr);
}

void losPrintDebug(const char *str)
{
    OutputDebugString(((std::string("PROGRAM DEBUG: ") += str) += "\n").c_str());
}

void losPrintError(const char *str)
{
    DWORD dwWritten;
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    std::string out("PROGRAM INFO: ");
    out += str;
    out += "\n";
    WriteConsole(hConsole, out.c_str(), DWORD(out.size()), &dwWritten, nullptr);
}