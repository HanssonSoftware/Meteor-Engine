/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "OutputContext.h"
#include <Types/String.h>
#include <Logging/LogMacros.h>

LOG_ADDCATEGORY(Vulkan);

#define STR(x) #x

void VulkanOutputContext::Clear()
{
	VulkanRegistry* vk = (VulkanRegistry*)registry;
	if (vk == nullptr)
	{
		MR_LOG(LogVulkan, Error, "Invalid RHIRegistry, clear command halted!");
		return;
	}

}

void VulkanOutputContext::Draw()
{
	VulkanRegistry* vk = (VulkanRegistry*)registry;
	if (vk == nullptr)
	{
		MR_LOG(LogVulkan, Error, "Invalid RHIRegistry, draw command halted!");
		return;
	}


	vkWaitForFences(vk->selectedDevice, 1, &renderFence, VK_TRUE, UINT64_MAX);
	vkResetFences(vk->selectedDevice, 1, &renderFence);

	static uint32 img = 0;
	vkAcquireNextImageKHR(vk->selectedDevice, vk->swapChain, UINT32_MAX, renderWaitSemaphore, 0, &img);

	VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

	VkCommandBufferBeginInfo cmdBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkResetCommandBuffer(cmdBuffer, 0);
	vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo);

	//VkClearColorValue cv = { 0.4f, 0.f, 1.f, 0.f };

	//VkImageSubresourceRange imgR = {};
	//imgR.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//imgR.baseArrayLayer = 0;
	//imgR.baseMipLevel = 0;
	//imgR.levelCount = 1;
	//imgR.layerCount = 1;

	//vkCmdClearColorImage(cmdBuffer, vk->swapChainImages[img], VK_IMAGE_LAYOUT_GENERAL, &cv, 1 , &imgR);

	VkClearValue cls = { 0.4f, 0.f, 1.f, 0.f };

	VkRenderPassBeginInfo rpInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
	rpInfo.clearValueCount = 1;
	rpInfo.pClearValues = &cls;
	rpInfo.framebuffer = vk->swapChainFramebuffers[img];
	rpInfo.renderPass = vk->renderPass;
	rpInfo.renderArea.offset = { 0, 0 };
	rpInfo.renderArea.extent = vk->surfaceCapabilities.maxImageExtent;

	vkCmdBeginRenderPass(cmdBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
	// ...
	vkCmdEndRenderPass(cmdBuffer);

	vkEndCommandBuffer(cmdBuffer);

	static constexpr VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo smInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
	smInfo.commandBufferCount = 1;
	smInfo.pCommandBuffers = &cmdBuffer;
	smInfo.pSignalSemaphores = &renderSharpSemaphore;
	smInfo.signalSemaphoreCount = 1;
	smInfo.pWaitSemaphores = &renderWaitSemaphore;
	smInfo.waitSemaphoreCount = 1;
	smInfo.pWaitDstStageMask = waitStages;
	
	VkResult a = vkQueueSubmit(vk->graphicsQueue, 1, &smInfo, 0);

	VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.pImageIndices = &img;
	presentInfo.pSwapchains = &vk->swapChain;
	presentInfo.swapchainCount = 1;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderSharpSemaphore;

	VkResult b = vkQueuePresentKHR(vk->graphicsQueue, &presentInfo);

	vkQueueWaitIdle(vk->graphicsQueue);
}

bool VulkanOutputContext::CreateCommandBuffers()
{
	VulkanRegistry* vk = (VulkanRegistry*)registry;
	if (vk == nullptr)
		return false;

	VkCommandPoolCreateInfo cmdPoolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	cmdPoolInfo.queueFamilyIndex = vk->QueueIndex;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkResult result = vkCreateCommandPool(vk->selectedDevice, &cmdPoolInfo, nullptr, &cmdPool);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateCommandPool returned: %s", STR(result));
		return false;
	}

	VkCommandBufferAllocateInfo cmdBufferInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	cmdBufferInfo.commandBufferCount = 1;
	cmdBufferInfo.commandPool = cmdPool;
	cmdBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkResult resultA = vkAllocateCommandBuffers(vk->selectedDevice, &cmdBufferInfo, &cmdBuffer);
	if (resultA != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkAllocateCommandBuffers returned: %s", STR(result));
		return false;
	}

	VkSemaphoreCreateInfo smInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

	VkResult a = vkCreateSemaphore(vk->selectedDevice, &smInfo, nullptr, &renderWaitSemaphore);
	VkResult b = vkCreateSemaphore(vk->selectedDevice, &smInfo, nullptr, &renderSharpSemaphore);

	VkFenceCreateInfo fcInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };

	VkResult c = vkCreateFence(vk->selectedDevice, &fcInfo, nullptr, &renderFence);

	return true;
}

void VulkanOutputContext::CompileShader(const String name)
{

}

void VulkanOutputContext::CleanUp() const
{
	VulkanRegistry* vk = (VulkanRegistry*)registry;
	if (vk == nullptr)
		return;

	vkDeviceWaitIdle(vk->selectedDevice);

	vkDestroyFence(vk->selectedDevice, renderFence, nullptr);

	vkDestroySemaphore(vk->selectedDevice, renderWaitSemaphore, nullptr);
	vkDestroySemaphore(vk->selectedDevice, renderSharpSemaphore, nullptr);

	vkFreeCommandBuffers(vk->selectedDevice, cmdPool, 1, &cmdBuffer);

	vkDestroyCommandPool(vk->selectedDevice, cmdPool, nullptr);
}


const String VulkanOutputContext::evaluateResultToText(VkResult Result) noexcept
{
	switch (Result)
	{
	case VK_SUCCESS:
		return "VK_SUCCESS";
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
		return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_FRAGMENTED_POOL:
		return "VK_ERROR_FRAGMENTED_POO";
	case VK_ERROR_UNKNOWN:
		return "VK_ERROR_UNKNOWN";
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		return "VK_ERROR_OUT_OF_POOL_MEMORY";
	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
		return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
	case VK_ERROR_FRAGMENTATION:
		return "VK_ERROR_FRAGMENTATION";
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
		return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
	case VK_PIPELINE_COMPILE_REQUIRED:
		return "VK_PIPELINE_COMPILE_REQUIRED";
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
	case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
		return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
		return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
	case VK_ERROR_NOT_PERMITTED_KHR:
		return "VK_ERROR_NOT_PERMITTED_KHR";
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
		return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
	case VK_THREAD_IDLE_KHR:
		return "VK_THREAD_IDLE_KHR";
	case VK_THREAD_DONE_KHR:
		return "VK_THREAD_DONE_KHR";
	case VK_OPERATION_DEFERRED_KHR:
		return "VK_OPERATION_DEFERRED_KHR";
	case VK_OPERATION_NOT_DEFERRED_KHR:
		return "VK_OPERATION_NOT_DEFERRED_KHR";
	case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
		return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
	case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
		return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
	case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
		return "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
	default:
		break;
	}

	return "Undefined Error!";
}