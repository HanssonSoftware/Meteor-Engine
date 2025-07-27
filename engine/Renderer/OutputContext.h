/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include "Registry.h"

struct String;
#pragma warning(disable : 26495) // Variable 'X::Y' is uninitialized. Always initialize a member variable (type.6).

class VulkanOutputContext
{
	friend class VulkanRegistry;
public:
	VulkanOutputContext() = default;

	virtual void Clear();

	virtual void Draw();

	virtual void CleanUp() const;

	bool CreateCommandBuffers();
	
	virtual void CompileShader(const String name);

protected:
	VulkanRegistry* registry;
	VulkanDevice* owningDevice;

	VkCommandBuffer cmdBuffer;

	VkCommandPool cmdPool;

	VkSemaphore renderWaitSemaphore;

	VkFence renderFence;

	VkSemaphore renderSharpSemaphore;
};

