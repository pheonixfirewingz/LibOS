// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/FileIO.h>
#include <string>
#include <algorithm>

extern std::string platformGetCurrentPath();

static std::string asset_path = "NOT_SET"; // Global variable to store the asset path

// Declares a function that sets the asset path to be used by the library
losResult losSetAssetPath(const char *path)
{
    asset_path = path;  // Sets the asset path
    return LOS_SUCCESS; // Returns success status
}

// A utility function to get the correct path of the file based on platform and asset path
std::string getCorrectPath(const std::string path)
{
    std::string corrected_path = path;                  // Creates a copy of the path to modify it
    size_t pos = corrected_path.find("$[binary_base]"); // Searches for "$[binary_base]" string in the path
    if (pos != std::string::npos)                       // If found
        corrected_path.replace(pos, std::string("$[binary_base]").length(),
                               platformGetCurrentPath()); // Replaces it with the platform-specific path of the binary
    pos = corrected_path.find("$[asset_path]");           // Searches for "$[asset_path]" string in the path
    if (pos != std::string::npos)                         // If found
        corrected_path.replace(pos, std::string("$[asset_path]").length(),
                               asset_path);              // Replaces it with the asset path

    std::replace(corrected_path.begin(), corrected_path.end(), '\\', '/');
    return corrected_path; // Returns the corrected path
}