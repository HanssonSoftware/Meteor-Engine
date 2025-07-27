/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include "Queue.h"
#include <Platform/PlatformDefs.h>

struct String;
class VulkanSwapChain;
class VulkanOutputContext;
class VulkanDevice;

class VulkanRegistry
{
	friend class VulkanOutputContext;
	friend class VulkanSwapChain;
public:
	VulkanRegistry() = default;
	
	virtual ~VulkanRegistry() noexcept = default;

	static String GetName();

	static bool Initialize();

	static void Shutdown();

	void Render();

	static VulkanRegistry* GetRegistry();

	VkInstance GetInstance() const { return instance; }

protected:

	bool CreateInstance();

	void CreateDebugMessenger();

	void DestroyDebugMessenger() const;

	bool FindQueues();

	bool CreateDevice();

	bool CreateRenderpass();

	//bool CreateSwapchain();

	//bool CreateImageViews();

	//bool CreateFramebuffers();
	//


	VkInstance instance;

	VkDebugUtilsMessengerEXT debugger;

	VkShaderModule CreateShader(const VkShaderModule fromModule) const {};

	//VkShaderModule OpenShader(const String path) const;
	

protected:
	static inline VulkanRegistry* registry;

	VulkanOutputContext* outputContext;

	VulkanSwapChain* swapChain;

	VulkanDevice* device;

	VkRenderPass renderPass;
};


constexpr const char* VkResultToText(const VkResult& result);