// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Error.h>
#include <libos/FileIO.h>
#include <string>

// Returns the current binary directory as a string.
std::string platformGetCurrentPath()
{
    return "path";
}

std::string platformGetHomePath()
{
    return std::getenv("HOME") != nullptr ? std::getenv("HOME") : "";
}