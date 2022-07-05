#pragma once
#include <RefractileAPI.h>
#include "../InternalRefractile.h"
#include <thread>

struct LayoutObject
{
    const int64 position;
    const bool internal;
    const char* name;
    const bool has_mask;
    const char* mask_name;
    explicit LayoutObject(const int64 position, bool internal, const char* name, const bool has_mask, const char* mask) :            
                            position(position), internal(internal), name(name), has_mask(has_mask), mask_name(mask){};
};

struct refShaderProgram_T
{
    VkPipeline pipe;
    VkPipelineLayout pipe_layout;
    LayoutObject* input_vertex_layout[50];
    LayoutObject* input_fragment_layout[50];
    LayoutObject* output_vertex_layout[50];
    LayoutObject* output_fragment_layout[50];
};

struct refDrawPass_T
{
    VkRenderPass pass;
};

struct refFrameBuffer_T
{
    VkFramebuffer buffer;
};

struct refCommandBuffer_T
{
    std::thread::id creator;
    VkCommandPool command_pool;
    VkCommandBuffer buffer;
    VkFence render_fence;
    uint32 index = 0;
};

losResult refCreateFrameBuffer(refHandle handle,refFrameBuffer * buffer,VkImageView* list);