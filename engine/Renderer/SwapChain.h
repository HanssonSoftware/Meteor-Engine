/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/Vector.h>
#include "Vulkan.h"

class VulkanSwapChain
{
public:
	virtual void Init();

	virtual void Shutdown();

	virtual void Present();

	virtual Vector2<float> GetSize() const noexcept;

protected:
	VkSurfaceKHR surface;

	VkSwapchainKHR object;
};

