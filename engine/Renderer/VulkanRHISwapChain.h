/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once

#include <vulkan/vulkan.h>

class VulkanRHISwapChain : public IRHISwapChain
{
public:
	virtual void Init() override;

	virtual void Shutdown() override;

	virtual void Present() override;

	virtual Vector2<float> GetSize() const noexcept override;

protected:
	VkSurfaceKHR surface;

	VkSwapchainKHR object;
};

