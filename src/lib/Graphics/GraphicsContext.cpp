#define VMA_IMPLEMENTATION 1

#include <RefractileAPI.h>

#include <cstdio>
#include <vector>

#include "Components/Defines.h"
#include "vkExternal.h"
#include <vulkan/vulkan_core.h>

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

losResult refAppendGraphicsContext(refHandle handle, losWindow window)
{
    //instance
    {
        vkb::InstanceBuilder inst_builder;
        auto result = inst_builder.set_app_name("LibOS_app").set_engine_name("LibOS RefractileAPI Api Abstraction").set_engine_version(0,0,2)
        .require_api_version(VK_API_VERSION_1_3).use_default_debug_messenger().build();

        if (!result)                                  
        {                                                                         
            std::printf("LIB OS: Vulkan Error: %s\n", result.error().message().data()); 
            return LOS_ERROR_COULD_NOT_INIT;                                                           
        }
        handle->instance = result.value();
    }
    //surface
    {
        VkResult result;
#if CMAKE_SYSTEM_NUMBER == 0
    const VkWaylandSurfaceCreateInfoKHR surface_info{VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR, nullptr, 0,
                                                     window->display, window->surface};

    PASSED(vkCreateWaylandSurfaceKHR((*handle).instance, &surface_info, nullptr, &(*handle).surface),
           LOS_ERROR_HANDLE_IN_USE);
#endif
#if CMAKE_SYSTEM_NUMBER == 1
    const VkWin32SurfaceCreateInfoKHR surface_info{VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, nullptr, 0,
                                                     GetModuleHandle(nullptr),window->win_hand};

    if((result = vkCreateWin32SurfaceKHR(handle->instance, &surface_info, nullptr, &handle->surface)) != VK_SUCCESS)
    {
        std::printf("LIB OS: Vulkan Error: %s\n", getError(result)); 
        return LOS_ERROR_COULD_NOT_INIT;
    }
#endif
    }
    //physical device
    {
        vkb::PhysicalDeviceSelector phys_dev_selector (handle->instance);
        auto result = phys_dev_selector.set_surface(handle->surface).select();

        if (!result)                                  
        {                                                                         
            std::printf("LIB OS: Vulkan Error: %s\n", result.error().message().data()); 
            return LOS_ERROR_COULD_NOT_INIT;                                                           
        }
        handle->physical = result.value();
    }
    {
        vkb::DeviceBuilder device_builder (handle->physical);
        auto result = device_builder.build();

        if (!result)                                  
        {                                                                         
            std::printf("LIB OS: Vulkan Error: %s\n", result.error().message().data()); 
            return LOS_ERROR_COULD_NOT_INIT;                                                           
        }

        handle->device = result.value();
    }
    //memory management
    {
        VkResult result;
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_3;
        allocatorInfo.physicalDevice = (*handle).physical;
        allocatorInfo.device = (*handle).device;
        allocatorInfo.instance = (*handle).instance;

        if((result = vmaCreateAllocator(&allocatorInfo, &(*handle).vulkan_allocator))  != VK_SUCCESS)
        {
            std::printf("LIB OS: Vulkan Error: %s\n", getError(result)); 
            return LOS_ERROR_COULD_NOT_INIT;
        }
    }
    //swap_chain
    {
        vkb::SwapchainBuilder swapchain_builder(handle->device);
        auto result = swapchain_builder.build();
        
        if (!result)                                  
        {                                                                         
            std::printf("LIB OS: Vulkan Error: %s\n", result.error().message().data()); 
            return LOS_ERROR_COULD_NOT_INIT;                                                           
        }

        handle->swap_chain = result.value();

        window->resize_callback = [](refHandle handle,uint64 width,uint64 height)
        {
            (void)width; 
            (void)height;
            vkb::SwapchainBuilder swap_chain_builder(handle->device);
            auto result = swap_chain_builder.set_old_swapchain(handle->swap_chain).build();
        
            if (!result)                                  
            {                                                                         
                std::printf("LIB OS: Vulkan Error: %s\n", result.error().message().data()); 
                return LOS_ERROR_COULD_NOT_INIT;                                                           
            }

            handle->swap_chain = result.value();
            return LOS_SUCCESS;
        };
    }
    return LOS_SUCCESS;
}

losResult refUnAppendGraphicsContext(refHandle handle)
{
    vkb::destroy_swapchain(handle->swap_chain);
    vmaDestroyAllocator(handle->vulkan_allocator);
    vkb::destroy_device(handle->device);
    vkDestroySurfaceKHR(handle->instance, handle->surface, nullptr);
    vkb::destroy_instance(handle->instance);
    return LOS_SUCCESS;
}