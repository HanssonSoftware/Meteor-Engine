/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <RHI/RHIRegistry.h>
//#include "VulkanRHIOutputContext.h"
#include <vulkan/vulkan.h>

#define APPEND_PRIV(text) \
	L#text

#define LINE_PRIV __LINE__
#define MERGEFUNC(x, y) x##y
#define MERGEFUNC2(x, y) MERGEFUNC(x, y)

#define VK_CHECK(result, func) \
	VkResult MERGEFUNC2(func, LINE_PRIV) = result; bool MERGEFUNC2(boolean, LINE_PRIV) = MERGEFUNC2(func, LINE_PRIV) != VK_SUCCESS; if (MERGEFUNC2(boolean, LINE_PRIV)) { MR_LOG(LogVulkan, Error, "%s returned: %s", #func, evaluateResultToText(MERGEFUNC2(func, LINE_PRIV)).Chr()); } return !MERGEFUNC2(boolean, LINE_PRIV);


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

	struct 
	{
		VkQueue graphicsQueue;
		int graphicsQueueFamily = -1;
		int graphicsQueueFamilyIndex = -1;

		VkQueue presentationQueue;
		int presentationQueueFamily = -1;
		int presentationQueueFamilyIndex = -1;
	} Queues;

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

	VkPipelineLayout pipelineLayout;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
};

