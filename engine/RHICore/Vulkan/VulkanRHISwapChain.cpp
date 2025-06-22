/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "VulkanRHISwapChain.h"
#include <Platform/PlatformLayout.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // _WIN64


void VulkanRHISwapChain::Init()
{
	//VkResult result = vkCreateWin32SurfaceKHR();

	VkSwapchainCreateInfoKHR info = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };

	//VkResult resultA = vkCreateSwapchainKHR();
}

void VulkanRHISwapChain::Shutdown()
{

}

void VulkanRHISwapChain::Present()
{

}

Vector2<float> VulkanRHISwapChain::GetSize() const noexcept
{
	if (object)
	{

	}

	return { -1.f, -1.f };
}
