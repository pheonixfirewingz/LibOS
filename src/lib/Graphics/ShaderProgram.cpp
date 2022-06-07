#include <RefractileAPI.h>
#include <Components/FileIO.h>
#include "../InternalRefractile.h"
#include <string>
#include <vector>
#include <string_view>
#include "Compiler/Compiler.h"
losResult readFile(const std::string path,void** data,size d_size)
{
    losResult res;
    losFileHandle fileHandle;
    losFileOpenInfo file;                                  
    file.fileBits = LOS_FILE_BIT_READ;                             
    file.path = path.data();                                      
    file.path_size = path.size();
    if ((res = losOpenFile(&fileHandle, file)) != LOS_SUCCESS) 
        return res;                                        
    if ((res = losReadFile(fileHandle,data,d_size)) != LOS_SUCCESS)          
        return res;                                        
    if ((res = losCloseFile(fileHandle)) != LOS_SUCCESS)       
        return res;

    return LOS_SUCCESS;
}


losResult refCreateShaderProgram(refHandle, refShaderProgram *, refCreateShaderProgramInfo & info)
{
    Compiler* compiler = new Compiler();
    losResult res;
    void* vertex_code;
    size vertex_code_size = 0;
    if((res = readFile(info.vertexShader,&vertex_code,vertex_code_size)) != LOS_SUCCESS)
    {
        printf("LIB OS: Vulkan Error: %s\n", "vertex shader failed to read");
        return res;
    }
    std::vector<uint32> v_shader_module = compiler->CompileGlslToSpv(static_cast<const char*>(vertex_code),vertex_code_size);

    void* fragment_code;
    size fragment_code_size = 0;
    if((res = readFile(info.fragmentShader,&fragment_code,fragment_code_size)) != LOS_SUCCESS)
    {
        printf("LIB OS: Vulkan Error: %s\n", "fragment shader failed to read");
        return res;
    }

    void* shader_layout;
    size shader_layout_size  = 0;
    if((res = readFile(info.shaderLayout,&shader_layout,shader_layout_size)) != LOS_SUCCESS)
    {
        printf("LIB OS: Vulkan Error: %s\n", "shader layout failed to read");
        return res;
    }
    return LOS_SUCCESS;
}

losResult refDestroyShaderProgram(refHandle, refShaderProgram)
{
    return LOS_SUCCESS;
}

