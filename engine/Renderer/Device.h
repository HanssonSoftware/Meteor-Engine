/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "Vulkan.h"

class VulkanDevice
{
	friend class VulkanRegistry;
	friend class VulkanSwapChain;
	friend class VulkanOutputContext;
public:
	VulkanDevice() = default;

	virtual ~VulkanDevice() noexcept = default;

protected:
	VkPhysicalDeviceFeatures deviceFeatures;

	VkPhysicalDevice physicalDevice;

	VkDevice device;

	VkQueue graphicsQueue;
	int graphicsQueueIndex = -1;
};

