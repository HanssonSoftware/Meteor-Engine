/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "VulkanRHIRegistry.h"
#include <RHI/RHIOutputContext.h>

struct String;
#pragma warning(disable : 26495) // Variable 'X::Y' is uninitialized. Always initialize a member variable (type.6).

class VulkanRHIOutputContext : public IRHIOutputContext
{
	friend class VulkanRHIRegistry;
public:
	VulkanRHIOutputContext(VulkanRHIRegistry* registry)
		: IRHIOutputContext(registry)
	{

	}

	virtual void Clear() override;

	virtual void Draw() override;

	virtual void CompileShader(const String name) override;
private:
	VkCommandBuffer cmdBuffer;

	VkSemaphore renderWaitSemaphore;

	VkSemaphore renderSharpSemaphore;
};

