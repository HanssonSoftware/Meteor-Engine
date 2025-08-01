/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include "Vulkan.h"

struct VulkanQueue
{
	inline constexpr operator VkQueue()
	{
		return handle;
	};

	inline constexpr operator VkQueue*()
	{
		return &handle;
	};

	int QueueFamilyIndex = -1;

	int QueueIndexInQueue = -1;

private:
	VkQueue handle;
};

