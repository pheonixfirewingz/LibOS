#include "vk_mem_alloc.h"
#include "../Window.h"
#include <Components/Defines.h>
#include <RefractileAPI.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_wayland.h>


struct refHandle_T
{
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swap_chain = VK_NULL_HANDLE;
};

losResult refCreateRefractileContext(refHandle *handle)
{
    if (!(*handle))
        return LOS_ERROR_HANDLE_IN_USE;

    (*handle) = new refHandle_T();
    return LOS_SUCCESS;
}

losResult refDestoryRefractileContext(refHandle handle);
{
    delete *handle;
    return LOS_SUCCESS;
}

losResult refAppendGraphicsContext(refHandle, losWindow)
{
    return LOS_SUCCESS;
}

losResult refUnAppendGraphicsContext(refHandle)
{
    return LOS_SUCCESS;
}