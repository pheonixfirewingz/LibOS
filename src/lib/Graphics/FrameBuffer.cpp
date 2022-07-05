#include "Share.h"

losResult refCreateFrameBuffer(refHandle,refFrameBuffer * buffer)
{
    if (!(*buffer))
        return LOS_ERROR_HANDLE_IN_USE;
    *buffer = new refFrameBuffer_T();
    return LOS_SUCCESS;
}

losResult refCreateFrameBuffer(refHandle handle,refFrameBuffer * buffer,VkImageView* colour)
{
    VkResult result;
    if (!(*buffer))
        return LOS_ERROR_HANDLE_IN_USE;
    *buffer = new refFrameBuffer_T();
    VkFramebufferCreateInfo frame_buffer_info{};
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.pNext = nullptr;
    frame_buffer_info.flags = 0;
    frame_buffer_info.renderPass = handle->pass;
    VkImageView list[2] = {*colour,handle->depth_image_view};
    if(handle->used_depth)
    {
        frame_buffer_info.attachmentCount = 2;
        frame_buffer_info.pAttachments = list;
    }
    else
    {
        frame_buffer_info.attachmentCount = 1;
        frame_buffer_info.pAttachments = colour;
    }
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle->physical, handle->surface, &surfaceCapabilities);
    frame_buffer_info.width = surfaceCapabilities.currentExtent.width;
    frame_buffer_info.height = surfaceCapabilities.currentExtent.height;
    frame_buffer_info.layers = 1;
    VK_TEST(vkCreateFramebuffer(handle->device,&frame_buffer_info,nullptr,&(*buffer)->buffer),LOS_ERROR_COULD_NOT_INIT);
    return LOS_SUCCESS;
}

losResult refDestroyFrameBuffer(refHandle handle,refFrameBuffer buffer)
{
    vkDestroyFramebuffer(handle->device, buffer->buffer,nullptr);
    return LOS_SUCCESS;
}
