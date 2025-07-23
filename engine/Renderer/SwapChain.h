/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/Vector.h>
#include "Vulkan.h"

class VulkanSwapChain
{
	friend class VulkanRegistry;
	friend class VulkanOutputContext;
public:
	VulkanSwapChain() = default;

	virtual ~VulkanSwapChain() noexcept = default;

	virtual bool Init();

	virtual void Shutdown();

	virtual void Present();

	virtual Vector2<uint32> GetSize() const noexcept { return size; };

protected:
	bool CreateSurface();

	bool CreateSwapchain();

	bool CreateFramebuffers();

	Vector2<uint32> size;

	uint32 swapChainImagesCount = 0;
	VkImage* images = nullptr;
	VkImageView* imagesViews = nullptr;
	VkFramebuffer* framebuffers = nullptr;

	VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
	VkSurfaceKHR surface;

	VkSwapchainKHR object;
};

