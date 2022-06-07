#pragma once
#include <Components/Defines.h>
#include <vector>
#include <string>

class Compiler
{
    std::vector<std::string> lexerSource(const char* source,const size source_size);
    public:
    Compiler() = default;
    ~Compiler() = default;
    std::vector<uint32> CompileGlslToSpv(const char* source,const size source_size);
};