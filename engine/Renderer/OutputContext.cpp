/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "OutputContext.h"
#include "Device.h"
#include "SwapChain.h"
#include <Types/String.h>
#include <Logging/LogMacros.h>

LOG_ADDCATEGORY(Vulkan);

void VulkanOutputContext::Clear()
{

}

void VulkanOutputContext::Draw()
{
	vkWaitForFences(owningDevice->device, 1, &renderFence, VK_TRUE, UINT64_MAX);
	vkResetFences(owningDevice->device, 1, &renderFence);

	static uint32 img = 0;
	vkAcquireNextImageKHR(owningDevice->device, registry->swapChain->object, UINT32_MAX, renderWaitSemaphore, 0, &img);

	constexpr VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

	constexpr VkCommandBufferBeginInfo cmdBufferBeginInfo = { 
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,			// sType
		nullptr,												// pNext
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,			// flags
		nullptr													// pInheritanceInfo
	
	};

	vkResetCommandBuffer(cmdBuffer, 0);
	vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo);

	VkClearValue cls = { 0.4f, 0.f, 1.f, 0.f };

	VkRenderPassBeginInfo rpInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
	rpInfo.clearValueCount = 1;
	rpInfo.pClearValues = &cls;
	rpInfo.framebuffer = registry->swapChain->framebuffers[img];
	rpInfo.renderPass = registry->renderPass;
	rpInfo.renderArea.offset = { 0, 0 };
	rpInfo.renderArea.extent = { registry->swapChain->size.x, registry->swapChain->size.y };

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
	
	VkResult a = vkQueueSubmit(owningDevice->graphicsQueue, 1, &smInfo, 0);

	VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.pImageIndices = &img;
	presentInfo.pSwapchains = &registry->swapChain->object;
	presentInfo.swapchainCount = 1;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderSharpSemaphore;

	VkResult b = vkQueuePresentKHR(owningDevice->graphicsQueue, &presentInfo);

	vkQueueWaitIdle(owningDevice->graphicsQueue);
}

bool VulkanOutputContext::CreateCommandBuffers()
{
	registry = VulkanRegistry::GetRegistry();
	owningDevice = registry->device;

	const VkCommandPoolCreateInfo cmdPoolInfo = { 
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,				// sType
		nullptr,												// pNext
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,		// flags
		(uint32)owningDevice->graphicsQueueIndex				// queueFamilyIndex
	};

	const VkResult result = vkCreateCommandPool(owningDevice->device, &cmdPoolInfo, nullptr, &cmdPool);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateCommandPool returned: %s", VkResultToText(result));
		return false;
	}

	VkCommandBufferAllocateInfo cmdBufferInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	cmdBufferInfo.commandBufferCount = 1;
	cmdBufferInfo.commandPool = cmdPool;
	cmdBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkResult resultA = vkAllocateCommandBuffers(owningDevice->device, &cmdBufferInfo, &cmdBuffer);
	if (resultA != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkAllocateCommandBuffers returned: %s", VkResultToText(result));
		return false;
	}

	constexpr VkSemaphoreCreateInfo smInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

	VkResult a = vkCreateSemaphore(owningDevice->device, &smInfo, nullptr, &renderWaitSemaphore);
	VkResult b = vkCreateSemaphore(owningDevice->device, &smInfo, nullptr, &renderSharpSemaphore);

	constexpr VkFenceCreateInfo fcInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };

	VkResult c = vkCreateFence(owningDevice->device, &fcInfo, nullptr, &renderFence);

	return true;
}

void VulkanOutputContext::CompileShader(const String name)
{

}

void VulkanOutputContext::CleanUp() const
{
	VulkanRegistry* registry = VulkanRegistry::GetRegistry();
	VulkanDevice* device = registry->device;

	vkDeviceWaitIdle(device->device);

	vkDestroyFence(device->device, renderFence, nullptr);

	vkDestroySemaphore(device->device, renderWaitSemaphore, nullptr);
	vkDestroySemaphore(device->device, renderSharpSemaphore, nullptr);

	vkFreeCommandBuffers(device->device, cmdPool, 1, &cmdBuffer);

	vkDestroyCommandPool(device->device, cmdPool, nullptr);
}
