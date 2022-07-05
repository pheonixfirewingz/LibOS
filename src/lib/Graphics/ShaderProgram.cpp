#include "Share.h"
#include <Components/FileIO.h>
#include <string>
#include <vector>
#include <algorithm>
#include <json/json.h>

losResult readFile(const std::string path,void** data,size* d_size)
{
    losResult res;
    losFileHandle fileHandle;
    losFileOpenInfo file;                                  
    file.fileBits = LOS_FILE_BIT_READ | LOS_FILE_BIT_BINARY;                             
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

losResult createShaderModule(refHandle handle,VkResult* result,VkShaderModule* module,const char* path) 
{
    losResult res;
    uint32* code;
    size code_size = 0;
    if((res = readFile(path,reinterpret_cast<void**>(&code),&code_size)) != LOS_SUCCESS)
    {
        printf("LIB OS: FileIO Error: %s \"%s\"\n", "vertex shader failed path:",path);
        return res;
    }

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code_size;
    createInfo.pCode = code;
    
    if ((*result = vkCreateShaderModule(handle->device, &createInfo, nullptr, module)) != VK_SUCCESS) 
    {
        printf("LIB OS: Vulkan Error: %s \"%s\"\n", "Creating Module:",getError(*result));
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    }

    return LOS_SUCCESS;
}

losResult jsonToLayoutShaderObject(const Json::Value base,LayoutObject** program_input,LayoutObject** program_output,const char * path)
{
    if(!base["Input"].isNull())
    {
        if(!base["Input"].isArray())
        {
            printf("LIB OS - Json Error: %s%s\n",path,".Input is not array");
            return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
        }
        uint8 index = 0;
        for(auto object : base["Input"])
        {
            if(index > 50) break;
            (void)object;
            int64 position = -1;
            bool internal = false;
            std::string name = "";
            bool has_mask = false;
            std::string mask_name = "";
            if(!object["RasterIndex"].isNull())
            {
                internal = true;
                if((!object["RasterIndex"].isIntegral()))
                {
                    printf("LIB OS - Json Error: %s.Input.obj[%x]%s\n",path,index,".(RasterIndex|Position) is not an int");
                    return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
                }
                position = static_cast<int64>(object["RasterIndex"].asInt64());
            }
            else if(!object["Position"].isNull())
            {
                if(!object["Position"].isIntegral())
                {
                    printf("LIB OS - Json Error: %s.Input.obj[%x]%s\n",path,index,".(RasterIndex|Position) is not an int");
                    return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
                }
                position = static_cast<int64>(object["Position"].asInt64());
            }
            else 
            {
                printf("LIB OS - Json Error: %s.Input.obj[%x]%s\n",path,index,".(RasterIndex|Position) is not defined");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }

            if(!object["Name"].isNull())
            {
                if(!object["Name"].isString())
                {
                    printf("LIB OS - Json Error: %s.Input.obj[%x]%s\n",path,index,".Name is not an int");
                    return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
                }
                name = static_cast<std::string>(object["Name"].asString());
            }
            else 
            {
                printf("LIB OS - Json Error: %s.Input.obj[%x]%s\n",path,index,".Name is not defined");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }

            if(!object["HasMask"].isNull())
            {
                if(!object["HasMask"].isBool())
                {
                    printf("LIB OS - Json Error: %s.Input.obj[%x]%s\n",path,index,".HasMask is not an bool");
                    return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
                }
                has_mask = object["HasMask"].asBool();
                if(has_mask)
                {
                    if(!object["Mask"].isNull())
                    {
                        if(!object["Mask"].isString())
                        {
                            printf("LIB OS - Json Error: %s.Input.obj[%x]%s\n",path,index,".Mask is not an int");
                            return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
                        }
                        mask_name = static_cast<std::string>(object["Mask"].asString());
                    }
                    else 
                    {
                        printf("LIB OS - Json Error: %s.Input.obj[%x]%s\n",path,index,".Mask is not defined");
                        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
                    }
                }
            }
            else 
            {
                printf("LIB OS - Json Error:%s.obj[%x]%s\n",path,index,".HasMask is not defined");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            program_input[index] = new LayoutObject(position,internal,std::move(name.c_str()),has_mask,std::move(mask_name.c_str()));
            index++;
        }
    }
    if(base["Output"].isNull() && !base["Output"].isArray())
    {
        printf("LIB OS - Json Error: %s%s\n",path,".Output not defined or is not an array");
        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
    }
    uint8 index = 0;
    for(const auto& object : base["Output"])
    {
        if(index > 50) break;
        int64 position = -1;
        bool internal = false;
        std::string name = "";
        bool has_mask = false;
        std::string mask_name = "";
        if(!object["RasterIndex"].isNull())
        {
            internal = true;
            if((!object["RasterIndex"].isIntegral()))
            {
                printf("LIB OS - Json Error: %s.Output.obj[%x]%s\n",path,index,".(RasterIndex|Position) is not an int");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            position = static_cast<int64>(object["RasterIndex"].asInt64());
        }
        else if(!object["Position"].isNull())
        {
            if(!object["Position"].isIntegral())
            {
                printf("LIB OS - Json Error: %s.Output.obj[%x]%s\n",path,index,".(RasterIndex|Position) is not an int");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            position = static_cast<int64>(object["Position"].asInt64());
        }
        else 
        {
            printf("LIB OS - Json Error: %s.Output.obj[%x]%s\n",path,index,".(RasterIndex|Position) is not defined");
            return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
        }

        if(!object["Name"].isNull())
        {
            if(!object["Name"].isString())
            {
                printf("LIB OS - Json Error: %s.Output.obj[%x]%s\n",path,index,".Name is not an int");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            name = static_cast<std::string>(object["Name"].asString());
        }
        else 
        {
            printf("LIB OS - Json Error: %s.Output.obj[%x]%s\n",path,index,".Name is not defined");
            return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
        }

        if(!object["HasMask"].isNull())
        {
            if(!object["HasMask"].isBool())
            {
                printf("LIB OS - Json Error: %s.Output.obj[%x]%s\n",path,index,".HasMask is not an bool");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            has_mask = object["HasMask"].asBool();
            if(has_mask)
            {
                if(!object["Mask"].isNull())
                {
                    if(!object["Mask"].isString())
                    {
                        printf("LIB OS - Json Error: %s.Output.obj[%x]%s\n",path,index,".Mask is not an int");
                        return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
                    }
                    mask_name = static_cast<std::string>(object["Mask"].asString());
                }
                else 
                {
                    printf("LIB OS - Json Error: %s.Output.obj[%x]%s\n",path,index,".Mask is not defined");
                    return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
                }
            }
        }
        else 
        {
            printf("LIB OS - Json Error:%s.obj[%x]%s\n",path,index,".HasMask is not defined");
            return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
        }
        program_output[index] = new LayoutObject(position,internal,std::move(name.c_str()),has_mask,std::move(mask_name.c_str()));
        index++;
    }
    return LOS_SUCCESS;
}

losResult refCreateShaderProgram(refHandle handle, refShaderProgram * program, refCreateShaderProgramInfo & info)
{
    VkResult result;
    if (!(*program))
        return LOS_ERROR_HANDLE_IN_USE;
    *program = new refShaderProgram_T();
    losResult res;
    void* shader_settings;
    size shader_settings_size  = 0;
    if((res = readFile(info.shader_settings,&shader_settings,&shader_settings_size)) != LOS_SUCCESS)
    {
        printf("LIB OS - Error: %s\n", "shader settings failed to read");
        return res;
    }
    std::vector<std::string> compiler_flags;
    {
        Json::Value root;
        Json::Reader reader;
        reader.parse(static_cast<const char*>(shader_settings), root);
        if(root["Layout"].isNull() && !root["Layout"].isObject())
        {
            printf("LIB OS - Json Error: %s\n","there is no layout defined or is not an object");
            return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
        }
        if(root["Compiler"].isNull() && !root["Compiler"].isObject())
        {
            printf("LIB OS - Json Error: %s\n","there is no compiler settings defined or is not an object");
            return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
        }
        {
            const Json::Value layout = root["Layout"];
            if(layout["Vertex"].isNull() && !layout["Vertex"].isObject())
            {
                printf("LIB OS - Json Error: %s\n","there is no Layout.Vertex defined or is not an object");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            if(layout["Fragment"].isNull() && !layout["Fragment"].isObject())
            {
                printf("LIB OS - Json Error: %s\n","there is no Layout.Vertex defined or is not an object");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            if((res = jsonToLayoutShaderObject(layout["Vertex"],(*program)->input_vertex_layout,(*program)->output_vertex_layout,"Layout.Vertex")) != LOS_SUCCESS)
                return res;
            if((res = jsonToLayoutShaderObject(layout["Fragment"],(*program)->input_fragment_layout,(*program)->output_fragment_layout,"Layout.Fragment")) != LOS_SUCCESS)
                return res;
        }
        {
            const Json::Value compiler = root["Compiler"];
            if(compiler["Mode"].isNull() && !compiler["Mode"].isString())
            {
                printf("LIB OS - Json Error: %s\n","there is no Compiler.Mode defined or is not a string");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            if(compiler["Optimize"].isNull() && !compiler["Optimize"].isBool())
            {
                printf("LIB OS - Json Error: %s\n","there is no Compiler.Optimize defined or is not a bool");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            if(compiler["Cash"].isNull() && !compiler["Cash"].isBool())
            {
                printf("LIB OS - Json Error: %s\n","there is no Compiler.Cash defined or is not a bool");
                return LOS_ERROR_COULD_NOT_GET_CORRECT_DATA;
            }
            if(compiler["Mode"].asString() == "Debug")
                compiler_flags.push_back("-M=Debug");
            else
                compiler_flags.push_back("-M=Release");

            if(compiler["Optimize"].asBool())
                compiler_flags.push_back("-O1");
            else
                compiler_flags.push_back("-O0");
            if(compiler["Cash"].asBool())
                compiler_flags.push_back("-K=true");
            else
                compiler_flags.push_back("-K=false");
        }
    }
    VkShaderModule vertex,fragment;
    (void)compiler_flags;
    if(info.pre_compiled)
    {
        if((res = createShaderModule(handle,&result,&vertex,info.vertex_shader)) != LOS_SUCCESS)
            return res;

        if((res = createShaderModule(handle,&result,&fragment,info.fragment_shader)) != LOS_SUCCESS)
            return res;
    }
    else return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertex;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragment;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    (void)shaderStages;
    
    
    vkDestroyShaderModule(handle->device,vertex,nullptr);
    vkDestroyShaderModule(handle->device,fragment,nullptr);
    return LOS_SUCCESS;
}

losResult refDestroyShaderProgram(refHandle handle, refShaderProgram program)
{
    vkDestroyPipeline(handle->device, program->pipe, nullptr);
    vkDestroyPipelineLayout(handle->device, program->pipe_layout, nullptr);
    return LOS_SUCCESS;
}

