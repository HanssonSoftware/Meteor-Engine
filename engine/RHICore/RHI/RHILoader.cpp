/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "RHILoader.h"
#include "RHIRegistry.h"
#include <Commandlet.h>
#include <Vulkan/VulkanRHIRegistry.h>
#include <D3D11/D3D11RHIRegistry.h>

IRHIRegistry* RHILoader::InitRHI(const SystemLayout layout)
{
	IRHIRegistry* reference = nullptr;

	switch (layout)
	{
	case SYSTEM_LAYOUT_WINDOWS:
		reference = InitWindows();
		break;
	case SYSTEM_LAYOUT_LINUX:
		break;
	case SYSTEM_LAYOUT_MACOS:
		break;
	case SYSTEM_LAYOUT_ANDROID:
		break;
	case SYSTEM_LAYOUT_IOS:
		break;

	}

	return reference;
}

inline IRHIRegistry* RHILoader::InitWindows() noexcept
{
	if (ICommandlet::Get().Expected<bool>("d3d11"))
	{
		return new D3D11RHIRegistry();
	}

	return new VulkanRHIRegistry();
}
