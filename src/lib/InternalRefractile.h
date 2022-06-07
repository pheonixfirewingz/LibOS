#pragma once
#include "Cmake.h"
#include "Callbacks.h"
#include "Graphics/vkExternal.h"
#include <AL/al.h>
#include <AL/alc.h>

struct refHandle_T
{
    VmaAllocator vulkan_allocator;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surface_format {VK_FORMAT_MAX_ENUM,VK_COLOR_SPACE_MAX_ENUM_KHR};
    VkPhysicalDevice physical;
    uint32 graphic_family_index;
    uint32 present_family_index;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkDevice device;
    VkSwapchainKHR swap_chain;
    std::vector<VkImage> swap_chain_images{};
    std::vector<VkImageView> swap_chain_image_views{};

    ALCcontext* audio_context;
};