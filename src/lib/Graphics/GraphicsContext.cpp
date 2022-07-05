#define VMA_IMPLEMENTATION 1

#include <RefractileAPI.h>
#include <Components/Defines.h>
#include "GraphicsWindow.h"

#include <cstdio>
#include <vector>

#include "Share.h"

#define PROFILE_NAME VP_LUNARG_DESKTOP_PORTABILITY_2021_NAME
#define PROFILE_SPEC_VERSION VP_LUNARG_DESKTOP_PORTABILITY_2021_SPEC_VERSION
losResult createSwapChain(refHandle handle,bool first);
void destroySwapchain(refHandle handle);

bool check(VkResult result,refHandle handle)
{
    if (result == VK_SUCCESS) return false;
    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        if(!hasWindowClosed())
        {
            destroySwapchain(handle);
            (void)createSwapChain(handle,false);
        }
        return false;
    }
    return true;
}

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
        printf("LIB OS: Vulkan Error Not Translatable: %d\n", result);
        return "";
    }
}

void createSwapChainInfo(refHandle& handle,VkSwapchainCreateInfoKHR* swap_chain_create_info,VkSwapchainKHR old_swap_chain) noexcept
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle->physical, handle->surface, &surfaceCapabilities);
    swap_chain_create_info->sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_chain_create_info->surface = handle->surface;
    swap_chain_create_info->imageFormat = handle->surface_format.format;
    swap_chain_create_info->imageColorSpace = handle->surface_format.colorSpace;

    uint32 image_count = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && image_count > surfaceCapabilities.maxImageCount) 
        image_count = surfaceCapabilities.maxImageCount;
    swap_chain_create_info->minImageCount = image_count;

    swap_chain_create_info->imageExtent = surfaceCapabilities.maxImageExtent;

    uint32 image_array_layers = 1;
	if (surfaceCapabilities.maxImageArrayLayers < 1)
		image_array_layers = surfaceCapabilities.maxImageArrayLayers;
    swap_chain_create_info->imageArrayLayers = image_array_layers;

    swap_chain_create_info->imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (handle->graphic_family_index !=handle->present_family_index) 
    {
        swap_chain_create_info->imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swap_chain_create_info->queueFamilyIndexCount = 2;
        uint32 queue_family_indices[] = { handle->graphic_family_index, handle->present_family_index };
        swap_chain_create_info->pQueueFamilyIndices = queue_family_indices;
	} 
    else 
		swap_chain_create_info->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        
    swap_chain_create_info->preTransform = surfaceCapabilities.currentTransform;
    swap_chain_create_info->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    std::vector<VkPresentModeKHR> available_resent_modes;
    const VkPresentModeKHR desired_present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR,VK_PRESENT_MODE_FIFO_KHR};
    uint32 present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(handle->physical,handle->surface,&present_mode_count,nullptr);
    available_resent_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(handle->physical,handle->surface,&present_mode_count,available_resent_modes.data());
	for (auto const& desired_pm : desired_present_modes) {
		for (auto const& available_pm : available_resent_modes) {
			if (desired_pm == available_pm) present_mode = desired_pm;
		}
	}
    swap_chain_create_info->presentMode = present_mode;
    swap_chain_create_info->clipped = VK_TRUE;
    swap_chain_create_info->oldSwapchain = old_swap_chain;
}

losResult createSwapChain(refHandle handle,bool first)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    VkResult result;
    vkQueueWaitIdle(handle->graphics_queue);
    //depth stencil image
    if(handle->used_depth)
    {
        VkImageCreateInfo dimg_info{};
        dimg_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        dimg_info.pNext = nullptr;
        dimg_info.flags = 0;
        dimg_info.imageType = VK_IMAGE_TYPE_2D;
        //FIXME: should be able to change from user
        dimg_info.format = VK_FORMAT_D32_SFLOAT;
        //-----------------------------------------
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle->physical, handle->surface, &surfaceCapabilities);
        dimg_info.extent.width = surfaceCapabilities.currentExtent.width;
        dimg_info.extent.height = surfaceCapabilities.currentExtent.height;
        dimg_info.extent.depth = 1;
        dimg_info.mipLevels = 1;
        dimg_info.arrayLayers = 1;
        dimg_info.samples = static_cast<VkSampleCountFlagBits>(handle->sample_count);
        dimg_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        dimg_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        // for the depth image, we want to allocate it from GPU local memory
        VmaAllocationCreateInfo dimg_allocinfo = {};
        dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // allocate and create the image
        vmaCreateImage(handle->vulkan_allocator, &dimg_info, &dimg_allocinfo, &handle->depth_image, &handle->depth_image_alloc, nullptr);

        // build an image-view for the depth image to use for rendering
        VkImageViewCreateInfo dview_info{};
        dview_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        dview_info.pNext = nullptr;
        dview_info.flags = 0;
        dview_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        dview_info.image = handle->depth_image;
        //FIXME: should be able to change from user
        dview_info.format = VK_FORMAT_D32_SFLOAT;
        //-----------------------------------------
        dview_info.subresourceRange.baseMipLevel = 0;
        dview_info.subresourceRange.levelCount = 1;
        dview_info.subresourceRange.baseArrayLayer = 0;
        dview_info.subresourceRange.layerCount = 1;
        dview_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        VK_TEST(vkCreateImageView(handle->device, &dview_info, nullptr, &handle->depth_image_view),LOS_ERROR_COULD_NOT_INIT);
    }
    //swap chain
    {
        VkSwapchainCreateInfoKHR swap_chain_create_info = {};
        VkSwapchainKHR swap_chain = handle->swap_chain;

        {
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle->physical, handle->surface, &surfaceCapabilities);
            handle->current_screen_size = std::move(surfaceCapabilities.currentExtent);
        }

        if(!first) createSwapChainInfo(handle,&swap_chain_create_info,swap_chain);
        else createSwapChainInfo(handle,&swap_chain_create_info,nullptr);

        VK_TEST(vkCreateSwapchainKHR(handle->device, &swap_chain_create_info, nullptr, &handle->swap_chain),LOS_ERROR_COULD_NOT_INIT);

        if(!first) vkDestroySwapchainKHR(handle->device, swap_chain, nullptr);

        uint32 image_count = 0;
        VK_TEST(vkGetSwapchainImagesKHR(handle->device, handle->swap_chain, &image_count, nullptr),LOS_ERROR_COULD_NOT_INIT);
        handle->swap_chain_images.resize(image_count);
        handle->swap_chain_image_views.resize(image_count);
        VK_TEST(vkGetSwapchainImagesKHR(handle->device, handle->swap_chain, &image_count, handle->swap_chain_images.data()),LOS_ERROR_COULD_NOT_INIT);

        for (uint32_t i = 0; i < image_count; ++i)
        {
            VkImageViewCreateInfo image_view_create_info{};
            image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.image = handle->swap_chain_images[i];
            image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            image_view_create_info.format = handle->surface_format.format;
            image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            image_view_create_info.subresourceRange.baseMipLevel = 0;
            image_view_create_info.subresourceRange.levelCount = 1;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount = 1;
            VK_TEST(vkCreateImageView(handle->device, &image_view_create_info, nullptr, &handle->swap_chain_image_views[i]),LOS_ERROR_COULD_NOT_INIT);
            refFrameBuffer frame_buffer;
            losResult _result;
            if((_result = refCreateFrameBuffer(handle,&frame_buffer,&handle->swap_chain_image_views[i])) != LOS_SUCCESS)
                return _result;
            handle->framebuffer.emplace_back(std::move(frame_buffer));
        }
    }
    return LOS_SUCCESS;
}

void destroySwapchain(refHandle handle)
{
    vkDeviceWaitIdle(handle->device);
    if(handle->used_depth)
    {
        vmaDestroyImage(handle->vulkan_allocator,handle->depth_image,handle->depth_image_alloc);
        vkDestroyImageView(handle->device, handle->depth_image_view,nullptr);
    }
    for (auto frame: handle->framebuffer)
        refDestroyFrameBuffer(handle,frame);
    handle->framebuffer.clear();

    for (auto view : handle->swap_chain_image_views)
        vkDestroyImageView(handle->device, view, nullptr);
    handle->swap_chain_image_views.clear();
}

losResult refAppendGraphicsContext(refHandle handle, losWindow window,const refCreateGraphicContextInfo& info) noexcept
{
    VkResult result;
    handle->used_depth = info.has_depth_stencil;
    handle->sample_count = info.sample_count;
    const VpProfileProperties profile_properties = {PROFILE_NAME, PROFILE_SPEC_VERSION};
    //instance
    {
        // Check if the profile is supported at instance level
        VkBool32 profile_supported;
        result = vpGetInstanceProfileSupport(nullptr, &profile_properties, &profile_supported);

        if (!profile_supported)
        {
            printf("LIB OS: Vulkan Error: %s\n", "Required Profile Not Supported"); 
            return LOS_ERROR_COULD_NOT_INIT;
        }

        const VkApplicationInfo app_info{VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                       nullptr,"LibOS_app",VK_MAKE_VERSION(1, 0, 0),
                                       "RefractileAPI",VK_MAKE_VERSION(1, 0, 0),
                                       VK_API_VERSION_1_3};

        const char * extensions[] = {VK_KHR_SURFACE_EXTENSION_NAME,VK_KHR_WSI_SURFACE_EXTENSION_NAME};
        const VkInstanceCreateInfo inst_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, &app_info, 0, nullptr, 2, extensions};
        const VpInstanceCreateInfo i_info {&inst_info,&profile_properties,0};

        VK_TEST(vpCreateInstance(&i_info,nullptr,&handle->instance),LOS_ERROR_COULD_NOT_INIT)
    }
    //surface
    {
#if CMAKE_SYSTEM_NUMBER == 0
        const VkWaylandSurfaceCreateInfoKHR surface_info{VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR, nullptr, 0,
                                                     window->display, window->surface};

        VK_TEST(vkCreateWaylandSurfaceKHR((*handle).instance, &surface_info, nullptr, &(*handle).surface),LOS_ERROR_COULD_NOT_INIT)
#endif
#if CMAKE_SYSTEM_NUMBER == 1
        const VkWin32SurfaceCreateInfoKHR surface_info{VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, nullptr, 0,
                                                        GetModuleHandle(nullptr),window->win_hand};

        VK_TEST(vkCreateWin32SurfaceKHR(handle->instance, &surface_info, nullptr, &handle->surface),LOS_ERROR_COULD_NOT_INIT)
#endif
    }
    //physical device
    {
        uint32 p_count = 0;
        VK_TEST(vkEnumeratePhysicalDevices(handle->instance, &p_count, nullptr),LOS_ERROR_COULD_NOT_INIT)
        std::vector<VkPhysicalDevice> devices;
        devices.resize(p_count);
        VK_TEST(vkEnumeratePhysicalDevices(handle->instance, &p_count, devices.data()),LOS_ERROR_COULD_NOT_INIT)
        [[likely]]if(devices.size() == 1)
        {
            VkBool32 profile_supported;
            result = vpGetPhysicalDeviceProfileSupport(handle->instance,devices[0], &profile_properties, &profile_supported);

            if (!profile_supported)
            {
                printf("LIB OS: Vulkan Error: %s\n", "Required Profile Not Supported"); 
                return LOS_ERROR_COULD_NOT_INIT;
            }
            handle->physical = devices[0];
        }
        else
        {
            printf("LIB OS: Vulkan Error: %s\n", "Multi GPU Not Implied yet"); 
            return LOS_ERROR_COULD_NOT_INIT;
        }

        VkBool32 support{false};
        VK_TEST(vkGetPhysicalDeviceSurfaceSupportKHR(handle->physical, handle->graphic_family_index, handle->surface, &support),LOS_ERROR_COULD_NOT_INIT)
        [[unlikely]] if (!support)
        {
            printf("LIB OS: Vulkan Error: %s , %s\n", "something is not right with surface creation",getError(result)); 
            return LOS_ERROR_COULD_NOT_INIT;
        }
        uint32 count = 0;
        VK_TEST(vkGetPhysicalDeviceSurfaceFormatsKHR(handle->physical, handle->surface, &count, nullptr),LOS_ERROR_COULD_NOT_INIT)
        [[unlikely]]if (count == 0)
        {
            printf("LIB OS: Vulkan Error: %s\n", "no surface formats found"); 
            return LOS_ERROR_COULD_NOT_INIT;
        }
        std::vector<VkQueueFamilyProperties> queueFamilies;
        uint32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(handle->physical, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(handle->physical, &queueFamilyCount, queueFamilies.data());
        std::vector<VkSurfaceFormatKHR> formats(count);
        VK_TEST(vkGetPhysicalDeviceSurfaceFormatsKHR(handle->physical, handle->surface, &count, formats.data()),LOS_ERROR_COULD_NOT_INIT)


        for (const auto& available_format : formats)
        {
            if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                VkFormatProperties properties;
				vkGetPhysicalDeviceFormatProperties(handle->physical, VK_FORMAT_B8G8R8A8_SRGB, &properties);
				if ((properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
                   handle->surface_format = available_format;
            }
        }

        if(handle->surface_format.format == VK_FORMAT_UNDEFINED)
        {
            printf("LIB OS: Vulkan Error: %s\n", "no surface formats found"); 
            return LOS_ERROR_COULD_NOT_INIT;
        }

        uint32 i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            (void)queueFamily;
            VkBool32 presentSupport = false;
            VK_TEST(vkGetPhysicalDeviceSurfaceSupportKHR(handle->physical, i, handle->surface, &presentSupport),LOS_ERROR_COULD_NOT_INIT)
            if (presentSupport)
            {
                handle->present_family_index = i;
                break;
            }
            i++;
        }
        i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                handle->graphic_family_index = i;
                break;
            }
            i++;
        }            
    }
    //device
    {
        float queuePriority = 1.0f;
        const VkDeviceQueueCreateInfo queue_create_info = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr, 0, handle->graphic_family_index, 1, &queuePriority};
        const VkDeviceCreateInfo device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,nullptr,0,1,&queue_create_info,0,nullptr,0,nullptr,nullptr};
        const VpDeviceCreateInfo profile_device_info = {&device_create_info,&profile_properties,0};
        VK_TEST(vpCreateDevice(handle->physical,&profile_device_info,nullptr,&handle->device),LOS_ERROR_COULD_NOT_INIT)
        vkGetDeviceQueue(handle->device, handle->graphic_family_index, 0, &handle->graphics_queue);
        [[likely]]if (handle->present_family_index != handle->graphic_family_index)
        {
            vkGetDeviceQueue(handle->device, handle->present_family_index, 0, &handle->present_queue);
        } 
        else
        handle->present_queue = handle->graphics_queue;
    }
    //memory management
    {
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_3;
        allocatorInfo.physicalDevice = handle->physical;
        allocatorInfo.device = handle->device;
        allocatorInfo.instance = handle->instance;

        VK_TEST(vmaCreateAllocator(&allocatorInfo, &handle->vulkan_allocator),LOS_ERROR_COULD_NOT_INIT)
    }
    //render pass
    {
        VkRenderPassCreateInfo render_pass_info{};
        VkSubpassDescription subpass{};
        VkAttachmentDescription colour_attachment{};
        VkAttachmentReference colour_attachment_ref{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.pNext = nullptr;
        render_pass_info.flags = 0;
        colour_attachment.format = handle->surface_format.format;
        colour_attachment.samples = static_cast<VkSampleCountFlagBits>(info.sample_count);
        colour_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colour_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colour_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colour_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colour_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colour_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        colour_attachment_ref.attachment = 0;
        colour_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        if(info.has_depth_stencil)
        {
            VkAttachmentReference depth_attachment_ref{};
            depth_attachment_ref.attachment = 1;
            depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription depth_attachment{};
            //FIXME: should be able to change from user
            depth_attachment.format = VK_FORMAT_D32_SFLOAT;
            //-------------------------------
            depth_attachment.samples = static_cast<VkSampleCountFlagBits>(info.sample_count);
            depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            VkAttachmentDescription attachments[2] = {colour_attachment, depth_attachment};
            render_pass_info.attachmentCount = 2;
            render_pass_info.pAttachments = attachments;
            subpass.pDepthStencilAttachment = &depth_attachment_ref;
        }
        else
        {
            render_pass_info.attachmentCount = 1;
            render_pass_info.pAttachments = &colour_attachment;
        }

        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colour_attachment_ref;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        if(info.has_depth_stencil)
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        else
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
        render_pass_info.dependencyCount = 1;
        render_pass_info.pDependencies = &dependency;

        VK_TEST(vkCreateRenderPass(handle->device, &render_pass_info, nullptr, &handle->pass),LOS_ERROR_COULD_NOT_INIT);
    }
    losResult _result;
    if((_result = createSwapChain(handle,true)) != LOS_SUCCESS)
        return _result;

    window->resize_callback = [](refHandle handle,uint64, uint64) 
    {
        [[unlikely]]if(handle->closing) return LOS_SUCCESS;
        destroySwapchain(handle);
        return createSwapChain(handle,false);
    };
    //present semaphore
    {
        VkSemaphoreCreateInfo semaphore_create_info{};
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphore_create_info.pNext = nullptr;
        semaphore_create_info.flags = 0;
        VK_TEST(vkCreateSemaphore(handle->device, &semaphore_create_info, nullptr, &handle->present_semaphore),LOS_ERROR_COULD_NOT_INIT);
        VK_TEST(vkCreateSemaphore(handle->device, &semaphore_create_info, nullptr, &handle->render_semaphore),LOS_ERROR_COULD_NOT_INIT);
    }
    return LOS_SUCCESS;
}

losResult refUnAppendGraphicsContext(refHandle handle) noexcept
{
    handle->closing = true;
    vkDeviceWaitIdle(handle->device);
    destroySwapchain(handle);
    vkDestroyRenderPass(handle->device, handle->pass,nullptr);
    vkDestroySemaphore(handle->device, handle->render_semaphore,nullptr);
    vkDestroySemaphore(handle->device, handle->present_semaphore,nullptr);
    vmaDestroyAllocator(handle->vulkan_allocator);
    vkDeviceWaitIdle(handle->device);
    vkDestroyDevice(handle->device,nullptr);
    vkDestroySurfaceKHR(handle->instance, handle->surface, nullptr);
    vkDestroyInstance(handle->instance, nullptr);
    return LOS_SUCCESS;
}