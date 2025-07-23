/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "SwapChain.h"
#include "Registry.h"
#include "Device.h"
#include <Platform/PlatformLayout.h>
#include <Application.h>

LOG_ADDCATEGORY(Vulkan);

bool VulkanSwapChain::Init()
{
	return false;
}

void VulkanSwapChain::Shutdown()
{
	VulkanRegistry* registry = VulkanRegistry::GetRegistry();

	for (uint32 i = 0; i < swapChainImagesCount; i++)
	{
		if (imagesViews[i]) vkDestroyImageView(registry->device->device, imagesViews[i], nullptr);
		if (framebuffers[i]) vkDestroyFramebuffer(registry->device->device, framebuffers[i], nullptr);
	}

	delete[] images, imagesViews, framebuffers;

	if (object) vkDestroySwapchainKHR(registry->device->device, object, nullptr);

	if (surface) vkDestroySurfaceKHR(registry->instance, surface, nullptr);
}

void VulkanSwapChain::Present()
{

}

bool VulkanSwapChain::CreateSurface()
{
	VulkanRegistry* registry = VulkanRegistry::GetRegistry();

	VkWin32SurfaceCreateInfoKHR win32SurfaceInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
	if (WindowsWindowManager* windowManager = (WindowsWindowManager*)Application::Get()->GetWindowManager())
	{
		win32SurfaceInfo.hinstance = windowManager->GetInstance();
		win32SurfaceInfo.hwnd = (HWND)windowManager->GetFirstWindow()->GetWindowHandle();
	}
	else
	{
		MR_LOG(LogVulkan, Warn, "Unable to get window manager! Using fallback method!");

		win32SurfaceInfo.hinstance = GetModuleHandle(0);
		win32SurfaceInfo.hwnd = GetFocus();
	}

	const VkResult result = vkCreateWin32SurfaceKHR(registry->GetInstance(), &win32SurfaceInfo, nullptr, &surface);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateWin32SurfaceKHR returned: %s", VkResultToText(result));
		return false;
	}

	return true;
}

bool VulkanSwapChain::CreateSwapchain()
{
	VulkanRegistry* registry = VulkanRegistry::GetRegistry();
	VulkanDevice* device = registry->device;

	size = Application::Get()->GetFirstWindow()->GetSize();

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->physicalDevice, surface, &surfaceCapabilities);

	const VkExtent2D extent = { size.x, size.y };

	const VkSwapchainCreateInfoKHR swapChainInfo = { 
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,			// sType
		nullptr,												// pNext
		0,														// flags
		surface,												// surface
		surfaceCapabilities.minImageCount + 1,					// minImageCount
		VK_FORMAT_R8G8B8A8_UNORM,								// imageFormat
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,						// imageColorSpace
		extent,													// imageExtent
		1,														// imageArrayLayers
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |					// imageUsage
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,												
		VK_SHARING_MODE_EXCLUSIVE,								// imageSharingMode
		0,														// queueFamilyIndexCount
		nullptr,												// pQueueFamilyIndices
		surfaceCapabilities.currentTransform,					// preTransform
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,						// compositeAlpha
		VK_PRESENT_MODE_IMMEDIATE_KHR,							// presentMode
		1,														// clipped
		nullptr													// oldSwapchain
	};												   
		
	const VkResult result = vkCreateSwapchainKHR(device->device, &swapChainInfo, 0, &object);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateSwapchainKHR returned: %s", VkResultToText(result));
		return false;
	}

	
	vkGetSwapchainImagesKHR(device->device, object, &swapChainImagesCount, nullptr);

	images = new VkImage[swapChainImagesCount]();
	imagesViews = new VkImageView[swapChainImagesCount]();
	framebuffers = new VkFramebuffer[swapChainImagesCount]();

	vkGetSwapchainImagesKHR(device->device, object, &swapChainImagesCount, images);

	for (uint32 i = 0; i < swapChainImagesCount; i++)
	{
		constexpr VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		constexpr VkComponentMapping componentMapping = { VK_COMPONENT_SWIZZLE_IDENTITY };

		const VkImageViewCreateInfo createInfo = { 
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,		 // sType
			nullptr,										 // pNext
			0,												 // flags
			images[i],										 // image
			VK_IMAGE_VIEW_TYPE_2D,							 // viewType
			VK_FORMAT_R8G8B8A8_UNORM,						 // format
			componentMapping,								 // components
			subresourceRange								 // subresourceRange
		};

		const VkResult resultA = vkCreateImageView(device->device, &createInfo, nullptr, &imagesViews[i]);
		if (resultA != VK_SUCCESS)
		{
			MR_LOG(LogVulkan, Error, "vkCreateImageView[%i] returned: %s", i, VkResultToText(result));
			return false;
		}
	}

	return true;
}

bool VulkanSwapChain::CreateFramebuffers()
{
	VulkanRegistry* registry = VulkanRegistry::GetRegistry();

	for (uint32 i = 0; i < swapChainImagesCount; i++)
	{
		const VkFramebufferCreateInfo framebufferCreateInfo = { 
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,		// sType
			nullptr,										// pNext
			0,												// flags
			registry->renderPass,							// renderPass
			1,												// attachmentCount
			&imagesViews[i],								// pAttachments
			size.x,										    // width
			size.y,										    // height
			1												// layers
		};

		const VkResult result = vkCreateFramebuffer(registry->device->device, &framebufferCreateInfo, nullptr, &framebuffers[i]);
		if (result != VK_SUCCESS)
		{
			MR_LOG(LogVulkan, Error, "vkCreateFramebuffer returned: %s", VkResultToText(result));
			return false;
		}
	}

	return true;
}
