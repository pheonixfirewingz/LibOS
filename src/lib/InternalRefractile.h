#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wunused-function"
#include <vk_mem_alloc.h>
#pragma GCC diagnostic pop
#include <vulkan/vulkan_core.h>
#include <AL/al.h>
#include <AL/alc.h>

struct refHandle_T
{
    VmaAllocator vulkan_allocator;
    VkInstance instance;
    VkPhysicalDevice physical;
    VkDevice device;
    VkSurfaceKHR surface;
    VkSwapchainKHR swap_chain;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSurfaceFormatKHR surface_format {VK_FORMAT_MAX_ENUM,VK_COLOR_SPACE_MAX_ENUM_KHR};
};