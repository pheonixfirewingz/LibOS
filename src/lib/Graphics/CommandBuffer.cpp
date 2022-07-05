#include "Share.h"

inline bool threadCheck(refCommandBuffer& handle) noexcept
{
    if(handle->creator != std::this_thread::get_id())
        return false;
    return true;
}

losResult refCreateCommandBuffer(refHandle handle, refCommandBuffer * buffer)
{
    VkResult result;
    if (!(*buffer))
        return LOS_ERROR_HANDLE_IN_USE;
    *buffer = new refCommandBuffer_T();
    (*buffer)->creator = std::this_thread::get_id();
	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.pNext = nullptr;
	commandPoolInfo.queueFamilyIndex = handle->graphic_family_index;
	commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VK_TEST(vkCreateCommandPool(handle->device, &commandPoolInfo, nullptr, &(*buffer)->command_pool),LOS_ERROR_COULD_NOT_INIT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = (*buffer)->command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VK_TEST(vkAllocateCommandBuffers(handle->device, &allocInfo, &(*buffer)->buffer),LOS_ERROR_COULD_NOT_INIT);

    VkFenceCreateInfo fence_ci;
    fence_ci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_ci.pNext = nullptr;
    fence_ci.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VK_TEST(vkCreateFence(handle->device, &fence_ci, nullptr, &(*buffer)->render_fence),LOS_ERROR_COULD_NOT_INIT);
    return LOS_SUCCESS;
}

losResult refDestroyCommandBuffer(refHandle handle, refCommandBuffer buffer)
{
    threadCheck(buffer);
    vkDestroyCommandPool(handle->device, buffer->command_pool, nullptr);
    vkDestroyFence(handle->device, buffer->render_fence, nullptr);
    delete buffer;
    return LOS_SUCCESS;
}

losResult refBeginCommands(refHandle handle, refCommandBuffer buffer)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    VkResult result;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    VK_TEST(vkWaitForFences(handle->device, 1, &buffer->render_fence, true, 1000000000),LOS_ERROR_HANDLE_LOSSED);
    VK_TEST(vkResetFences(handle->device, 1, &buffer->render_fence),LOS_ERROR_HANDLE_LOSSED);
    VK_TEST(vkResetCommandBuffer(buffer->buffer, 0),LOS_ERROR_HANDLE_LOSSED);
    VK_TEST(vkAcquireNextImageKHR(handle->device, handle->swap_chain, 1000000000, handle->present_semaphore, nullptr, &buffer->index),LOS_ERROR_HANDLE_LOSSED);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = NULL;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(buffer->buffer, &beginInfo);
    return LOS_SUCCESS;
}

losResult refCmdBeginDrawing(refHandle handle, refCommandBuffer buffer,refFrameBuffer f_buffer,const float32 colours[4])
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    VkRenderPassBeginInfo rp_info{};
    rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_info.pNext = nullptr;
    rp_info.renderPass = handle->pass;
    rp_info.renderArea.offset.x = 0;
    rp_info.renderArea.offset.y = 0;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle->physical, handle->surface, &surfaceCapabilities);
    rp_info.renderArea.extent = {std::min<uint32_t>(surfaceCapabilities.currentExtent.width,handle->current_screen_size.width), 
                                 std::min<uint32_t>(surfaceCapabilities.currentExtent.height,handle->current_screen_size.height)};
    rp_info.clearValueCount = 2;
    if(f_buffer)
        rp_info.framebuffer = f_buffer->buffer;
    else
        rp_info.framebuffer = handle->framebuffer[buffer->index]->buffer;
    VkClearValue colour = {colours[0], colours[1], colours[2], colours[3]};
    VkClearValue depthClear = {};
    depthClear.depthStencil.depth = 1.f;
    VkClearValue clearValues[2] = {colour, depthClear};
    rp_info.pClearValues = &clearValues[0];
    vkCmdBeginRenderPass(buffer->buffer, &rp_info, VK_SUBPASS_CONTENTS_INLINE);
    return LOS_SUCCESS;
}

losResult refCmdBindVertexBuffer(refHandle handle, refCommandBuffer buffer, refDataBuffer)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    return LOS_SUCCESS;
}

losResult refCmdBindIndexBuffer(refHandle handle, refCommandBuffer buffer, refDataBuffer)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    return LOS_SUCCESS;
}

losResult refCmdBindShaderProgram(refHandle handle, refCommandBuffer buffer, refShaderProgram program)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    if(program->pipe == VK_NULL_HANDLE) return LOS_ERROR_HANDLE_LOSSED;
    vkCmdBindPipeline(buffer->buffer,VK_PIPELINE_BIND_POINT_GRAPHICS,program->pipe);
    return LOS_SUCCESS;
}

losResult refCmdDraw(refHandle handle, refCommandBuffer buffer,uint32 vertex_count,uint32 instance_count,uint32 first_vertex,uint32 first_instance)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    vkCmdDraw(buffer->buffer, vertex_count, instance_count, first_vertex, first_instance);
    return LOS_SUCCESS;
}

losResult refCmdDrawIndexed(refHandle handle, refCommandBuffer buffer,uint32 index_count,uint32 instance_count,uint32 first_index,uint32 vertex_offset,uint32 first_instance)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    vkCmdDrawIndexed(buffer->buffer,index_count,instance_count,first_index,vertex_offset,first_instance);
    return LOS_SUCCESS;
}

losResult refCmdEndDrawing(refHandle handle, refCommandBuffer buffer)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    vkCmdEndRenderPass(buffer->buffer);
    return LOS_SUCCESS;
}

losResult refEndCommands(refHandle handle, refCommandBuffer buffer)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    if(!threadCheck(buffer))
        return LOS_ERROR_TRIED_TO_ACCESS_THREAD_RESOURCES_ON_OTHER_THREAD;
    vkEndCommandBuffer(buffer->buffer);
    return LOS_SUCCESS;
}

losResult refExecuteCommands(refHandle handle, refCommandBuffer buffer,bool present_mode)
{
    [[unlikely]]if(handle->closing) return LOS_SUCCESS;
    VkResult result;
    if (present_mode)
    {
        VkSubmitInfo submit{};
        submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit.pNext = nullptr;
        submit.waitSemaphoreCount = 0;
        submit.pWaitSemaphores = nullptr;
        submit.pWaitDstStageMask = nullptr;
        submit.commandBufferCount = 1;
        submit.pCommandBuffers = &buffer->buffer;
        submit.signalSemaphoreCount = 0;
        submit.pSignalSemaphores = nullptr;
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submit.pWaitDstStageMask = &waitStage;
        submit.waitSemaphoreCount = 1;
        submit.pWaitSemaphores = &handle->present_semaphore;
        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &handle->render_semaphore;
        VK_TEST(vkQueueSubmit(handle->present_queue, 1, &submit, buffer->render_fence),LOS_ERROR_HANDLE_LOSSED);

        VkPresentInfoKHR present{};
        present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present.pNext = nullptr;
        present.waitSemaphoreCount = 1;
        present.pWaitSemaphores = &handle->render_semaphore;
        VkSwapchainKHR swapChains[] = {handle->swap_chain};
        present.swapchainCount = 1;
        present.pSwapchains = swapChains;
        present.pImageIndices = &buffer->index;
        present.pResults = nullptr;

        VK_TEST(vkQueuePresentKHR(handle->present_queue, &present),LOS_ERROR_HANDLE_LOSSED);
    }
    else
    {
        VkSubmitInfo submit{};
        submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit.pNext = nullptr;
        submit.waitSemaphoreCount = 0;
        submit.pWaitSemaphores = nullptr;
        submit.pWaitDstStageMask = nullptr;
        submit.commandBufferCount = 1;
        submit.pCommandBuffers = &buffer->buffer;
        submit.signalSemaphoreCount = 0;
        submit.pSignalSemaphores = nullptr;
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submit.pWaitDstStageMask = &waitStage;
        submit.waitSemaphoreCount = 1;
        submit.pWaitSemaphores = &handle->present_semaphore;
        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &handle->render_semaphore;
        VK_TEST(vkQueueSubmit(handle->graphics_queue, 1, &submit, buffer->render_fence),LOS_ERROR_HANDLE_LOSSED);
    }
    return LOS_SUCCESS;
}