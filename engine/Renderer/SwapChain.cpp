/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "SwapChain.h"
#include <Platform/PlatformLayout.h>


void VulkanSwapChain::Init()
{
	//VkResult result = vkCreateWin32SurfaceKHR();

	VkSwapchainCreateInfoKHR info = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };

	//VkResult resultA = vkCreateSwapchainKHR();
}

void VulkanSwapChain::Shutdown()
{

}

void VulkanSwapChain::Present()
{

}

Vector2<float> VulkanSwapChain::GetSize() const noexcept
{
	if (object)
	{

	}

	return { -1.f, -1.f };
}
