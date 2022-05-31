#include <RefractileAPI.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_wayland.h>
#define VMA_IMPLEMENTATION 1
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
#include <cstdio>
#include <vector>

#define DEBUG_VULKAN 1
#include "../InternalRefractile.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include "../Linux/Window.h"
#endif
#if CMAKE_SYSTEM_NUMBER == 1
#include "../Windows/Window.h"
#endif
#include "Components/Defines.h"

/*----------------------------------------------------------------
                                TODOLIST:
    1. add swap_chain funcionality to the mix
    2. add swap_chain resize to context + create callback resize for window resize
  ----------------------------------------------------------------*/

const char *getError(VkResult result)
{
    switch (result)
    {
    case VK_NOT_READY:
        return "VK_NOT_READY";
    case VK_TIMEOUT:
        return "VK_TIMEOUT";
    case VK_EVENT_SET:
        return "VK_EVENT_SET";
    case VK_EVENT_RESET:
        return "VK_EVENT_RESET";
    case VK_INCOMPLETE:
        return "VK_INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED:
        return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST:
        return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED:
        return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT:
        return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS:
        return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "VK_ERROR_FORMAT_NOT_SUPPORT";
    case VK_ERROR_FRAGMENTED_POOL:
        return "VK_ERROR_FRAGMENTED_POOL";
    case VK_ERROR_UNKNOWN:
        return "VK_ERROR_UNKNOWN";
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        return "VK_ERROR_OUT_OF_POOL";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_FRAGMENTATION:
        return "VK_ERROR_FRAGMENTATION";
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
        return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
    case VK_ERROR_SURFACE_LOST_KHR:
        return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR:
        return "VK_SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR:
        return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return "VK_ERROR_VALIDATION_FAILED_EXT";
    case VK_ERROR_INVALID_SHADER_NV:
        return "VK_ERROR_INVALID_SHADER_NV";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    case VK_ERROR_NOT_PERMITTED_EXT:
        return "VK_ERROR_NOT_PERMITTED_EXT";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
    case VK_THREAD_IDLE_KHR:
        return "VK_THREAD_IDLE_KHR";
    case VK_THREAD_DONE_KHR:
        return "VK_THREAD_DONE_KHR";
    case VK_OPERATION_DEFERRED_KHR:
        return "VK_OPERATION_DEFERRED_KHR";
    case VK_OPERATION_NOT_DEFERRED_KHR:
        return "VK_OPERATION_NOT_DEFERED_KHR";
    case VK_PIPELINE_COMPILE_REQUIRED_EXT:
        return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
    default:
        std::printf("LIB OS: Vulkan Error Not Translatable: %d\n", result);
        return "";
    }
}
#define PASSED(x, ret)                                                        \
    if ((function_result = x) != VK_SUCCESS)                                  \
    {                                                                         \
        std::printf("LIB OS: Vulkan Error: %s\n", getError(function_result)); \
        return ret;                                                           \
    }

losResult refAppendGraphicsContext(refHandle handle, losWindow window)
{
    VkResult function_result = VK_RESULT_MAX_ENUM;
    VkFormatProperties format_prop;
    uint32 v_count = 0, graphic_family_index = 0, present_family_index = 0;
    const uint32 api_version = VK_API_VERSION_1_3;
    const char *extensions[] = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME};
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    const VkApplicationInfo app_info{
        VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr,    "LibOS_app", VK_MAKE_VERSION(1, 0, 0), "LunaLux",
        VK_MAKE_VERSION(1, 0, 0),           api_version};

    const VkInstanceCreateInfo inst_info{
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, &app_info, 0, nullptr, 2, extensions};
#ifdef DEBUG_VULKAN_
    inst_info.enabledLayerCount = 1;
    const char *debugLayers[1] = {"VK_LAYER_KHRONOS_validation"};
    inst_info.ppEnabledLayerNames = debugLayers;
#endif
    PASSED(vkCreateInstance(&inst_info, nullptr, &(*handle).instance), LOS_ERROR_COULD_NOT_INIT)

    PASSED(vkEnumeratePhysicalDevices((*handle).instance, &v_count, nullptr), LOS_ERROR_COULD_NOT_INIT);
    std::vector<VkPhysicalDevice> devices(v_count);
    PASSED(vkEnumeratePhysicalDevices((*handle).instance, &v_count, devices.data()), LOS_ERROR_COULD_NOT_INIT);

    if (devices.size() == 1)
    {
        handle->physical = devices[0];
    }
    else
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;

    vkGetPhysicalDeviceQueueFamilyProperties(handle->physical, &v_count, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(v_count);
    vkGetPhysicalDeviceQueueFamilyProperties(handle->physical, &v_count, queueFamilies.data());

    {
        uint32 i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphic_family_index = i;
            }
            i++;
        }
    }

    const float32 queuePriority = 1.0f;
    const VkDeviceQueueCreateInfo queueCreateInfo{
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0, graphic_family_index, 1, &queuePriority};

    const VkDeviceCreateInfo deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                              nullptr,
                                              0,
                                              1,
                                              &queueCreateInfo,
                                              0,
                                              nullptr,
                                              static_cast<uint32_t>(deviceExtensions.size()),
                                              deviceExtensions.data(),
                                              nullptr};

    PASSED(vkCreateDevice((*handle).physical, &deviceCreateInfo, nullptr, &(*handle).device), LOS_ERROR_COULD_NOT_INIT);

    vkGetPhysicalDeviceFormatProperties((*handle).physical, VK_FORMAT_B8G8R8A8_UNORM, &format_prop);
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.vulkanApiVersion = api_version;
    allocatorInfo.physicalDevice = (*handle).physical;
    allocatorInfo.device = (*handle).device;
    allocatorInfo.instance = (*handle).instance;

    PASSED(vmaCreateAllocator(&allocatorInfo, &(*handle).vulkan_allocator), LOS_ERROR_COULD_NOT_INIT);

    const VkWaylandSurfaceCreateInfoKHR surface_info{VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR, nullptr, 0,
                                                     window->display, window->surface};

    PASSED(vkCreateWaylandSurfaceKHR((*handle).instance, &surface_info, nullptr, &(*handle).surface),
           LOS_ERROR_HANDLE_IN_USE);

    VkBool32 support{false};
    PASSED(vkGetPhysicalDeviceSurfaceSupportKHR((*handle).physical, graphic_family_index, (*handle).surface, &support),
           LOS_ERROR_COULD_NOT_INIT);
    if (!support)
    {
        std::printf("Lib OS - Vulkan: something is not right with surface creation");
        return LOS_ERROR_COULD_NOT_INIT;
    }

    VkSurfaceCapabilitiesKHR surfaceCapabilities{};
    PASSED(vkGetPhysicalDeviceSurfaceCapabilitiesKHR((*handle).physical, (*handle).surface, &surfaceCapabilities),
           LOS_ERROR_COULD_NOT_INIT);
    PASSED(vkGetPhysicalDeviceSurfaceFormatsKHR((*handle).physical, (*handle).surface, &v_count, nullptr),
           LOS_ERROR_COULD_NOT_INIT);
    std::vector<VkSurfaceFormatKHR> formats(v_count);
    PASSED(vkGetPhysicalDeviceSurfaceFormatsKHR((*handle).physical, (*handle).surface, &v_count, formats.data()),
           LOS_ERROR_COULD_NOT_INIT);

    for (const auto &availableFormat : formats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            (*handle).surface_format = availableFormat;
            break;
        }
    }

    if ((*handle).surface_format.format == VK_FORMAT_MAX_ENUM)
        (*handle).surface_format = formats[0];

    for (uint32 i = 0; i < queueFamilies.size(); i++)
    {
        VkBool32 presentSupport = false;
        PASSED(vkGetPhysicalDeviceSurfaceSupportKHR((*handle).physical, i, (*handle).surface, &presentSupport),
               LOS_ERROR_COULD_NOT_INIT);
        if (presentSupport)
            present_family_index = i;
    }

    vkGetDeviceQueue((*handle).device, graphic_family_index, 0, &(*handle).graphics_queue);
    if (present_family_index != graphic_family_index)
    {
        vkGetDeviceQueue((*handle).device, present_family_index, 0, &(*handle).present_queue);
    }
    else
        (*handle).present_queue = (*handle).graphics_queue;

    return LOS_SUCCESS;
}

losResult refUnAppendGraphicsContext(refHandle handle)
{
    vmaDestroyAllocator(handle->vulkan_allocator);
    vkDestroyDevice(handle->device, nullptr);
    handle->physical = VK_NULL_HANDLE;
    vkDestroySurfaceKHR(handle->instance, handle->surface, nullptr);
    vkDestroyInstance(handle->instance, nullptr);
    return LOS_SUCCESS;
}