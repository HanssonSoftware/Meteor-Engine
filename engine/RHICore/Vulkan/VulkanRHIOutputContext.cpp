/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "VulkanRHIOutputContext.h"
#include <Types/String.h>
#include <Logging/LogMacros.h>

LOG_ADDCATEGORY(Vulkan);

void VulkanRHIOutputContext::Clear()
{
	VulkanRHIRegistry* vk = (VulkanRHIRegistry*)registry;
	if (vk == nullptr)
	{
		MR_LOG(LogVulkan, Error, "Invalid RHIRegistry, clear command halted!");
		return;
	}

	const uint32 swapChainImagesQuantity = (uint32)vk->swapChainImages.size();
	for (uint32 i = 0; i < swapChainImagesQuantity; i++)
	{
		VkClearColorValue cls = {1.f, 1.f, 0.f, 0.f};
		VkImageSubresourceRange rgs = {};
		rgs.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		vkCmdClearColorImage(cmdBuffer, vk->swapChainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &cls, 1, &rgs);
	}
	
}

void VulkanRHIOutputContext::Draw()
{
	VulkanRHIRegistry* vk = (VulkanRHIRegistry*)registry;
	if (vk == nullptr)
	{
		MR_LOG(LogVulkan, Error, "Invalid RHIRegistry, draw command halted!");
		return;
	}

	VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	//vkCmdBindPipeline();
	//vkCmdBindVertexBuffers();
	//vkCmdBindIndexBuffer();
	//vkCmdDrawIndexed();
}

void VulkanRHIOutputContext::CompileShader(const String name)
{

}
