#include "Compiler.h"

std::vector<uint32> Compiler::CompileGlslToSpv(const char* source,const size source_size)
{
    std::vector<uint32> assembly;
    std::vector<std::string> lexed_source = lexerSource(source,source_size);

    return assembly;
}