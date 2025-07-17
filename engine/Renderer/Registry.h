/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "Queue.h"

class VulkanRegistry
{
	friend class VulkanOutputContext;
public:
	virtual String GetName() const;

	virtual bool Initialize();

	virtual void CleanUp() const;

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

	bool CreateFramebuffers();
	
	bool CreateRenderpass();

	VkInstance instance;

	VkDebugUtilsMessengerEXT dbMon;

	VkPhysicalDevice selectedVirtualDevice;

	int QueueIndex = -1;
	VkDevice selectedDevice;

	VkSurfaceKHR surface; // RHISwapChain

	VkSwapchainKHR swapChain; // RHISwapChain

	VkQueue graphicsQueue;

	VkImage* swapChainImages;

	VkImageView* swapChainImageViews;

	VkFramebuffer* swapChainFramebuffers;

	VkShaderModule shaderModuleFrag;

	VkShaderModule shaderModuleVert;

	VkPipeline pipeline;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;

	VkRenderPass renderPass;

	VkShaderModule CreateShader(const VkShaderModule fromModule) const {};

	VkShaderModule OpenShader(const String path) const;
	
	uint32 swapChainImagesCount = 0;
private:

};

