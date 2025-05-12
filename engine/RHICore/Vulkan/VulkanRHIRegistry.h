/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <RHI/RHIRegistry.h>
//#include "VulkanRHIOutputContext.h"
#include <vulkan/vulkan.h>


class VulkanRHIRegistry : public IRHIRegistry
{
	friend class VulkanRHIOutputContext;
public:
	virtual int GetMonitorIndex() const override;

	virtual String GetName() const override;

	virtual bool Initialize() override;

	virtual void CleanUp() const override;

protected:
	bool initVK();

	bool CreateInstance();

	bool GetInstanceLayerCompatibility();

	bool CreateDebugMessenger();

	bool CreateSurfaceWin32();

	bool CreateQueueSlots();

	bool CreateDevice();

	bool CreateSwapchain();

	bool CreateImageViews();

	const String evaluateResultToText(VkResult Result) noexcept;

	VkInstance instance;

	VkDebugUtilsMessengerEXT dbMon;

	VkPhysicalDevice selectedVirtualDevice;

	int selectedQueueIndex = -1;
	int selectedQueueFamilyCount = 0;
	VkDevice selectedDevice;

	VkSurfaceKHR surface;

	VkSwapchainKHR swapChain;

	std::vector<VkImage> swapChainImages;

	std::vector<VkImageView> swapChainImageViews;

	VkShaderModule shaderModuleFrag;

	VkShaderModule shaderModuleVert;

	VkQueue graphicsQueue;

	VkPipelineLayout pipelineLayout;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
};

