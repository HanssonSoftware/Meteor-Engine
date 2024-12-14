/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
//#define _CRT_SECURE_NO_WARNINGS
//#include "../GraphicsDevice.h"
//#include <Log/LogMacros.h>
//#include <vector>
//#include <vulkan/vulkan.h>
//#ifdef _WIN32
//#include <vulkan/vulkan_win32.h>  
//#endif // _WIN32
//#include <Types/PlatformDefs.h>
//LOG_ADDCATEGORY(Vulkan);
//
//class String;
//
//// Add our supported layers
//static constexpr const char* requiredMinimalLayers[] =
//{
//	"VK_LAYER_KHRONOS_validation"
//	//"VK_LAYER_AMD_switchable_graphics"
//};
//
//static constexpr unsigned int requiredMinimalLayersCount = std::size(requiredMinimalLayers);
//
//// Add our supported device extensions
//static constexpr const char* requiredMinimalInstanceExtensions[] =
//{
//	VK_KHR_SURFACE_EXTENSION_NAME,
//#ifdef _WIN32
//	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
//#endif // _WIN32
//
//#ifdef MR_DEBUG
//	VK_EXT_DEBUG_UTILS_EXTENSION_NAME
//#endif // MR_DEBUG
//};
//
//// Add our supported instance extensions
//static constexpr const char* requiredMinimalDeviceExtensions[] =
//{
//	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
//};
//
//static constexpr unsigned int requiredMinimalDeviceExtensionsCount = std::size(requiredMinimalDeviceExtensions);
//static constexpr unsigned int requiredMinimalInstanceExtensionsCount = std::size(requiredMinimalInstanceExtensions);
//
///*
///******		Vulkan graphical interface		******
//
//	* Child class from IGraphicsDevice
//	* vkXXX functions must be called twice, first for enumerate (counting), second for actual data to fill.
//	* All functions are need to be inlined except inherited members/functions/constants from base.
//	
//*/
//class IVulkanDevice : public IGraphicsDevice
//{
//public:
//	IVulkanDevice();
//
//	virtual ~IVulkanDevice() override;
//
//	// Inherited via IGraphicsDevice
//	virtual bool Init() override;
//
//	virtual void cleanUp() override;
//
//	virtual bool Render(float deltaTime) override;
//
//	virtual const String getRendererSignatature() const;
//private:
//	inline void initInstance();
//
//	inline void createDebugMessenger();
//
//	inline void selectDevice();
//
//	inline void initDevices();
//
//	inline void createSurface();
//
//	inline void createSwapChain();
//
//	inline void createSemaphoresAndFences();
//	
//	/** "Converts" the result code to text, which can be pasted to log or assertion error message.*/
//	const String evaluateResultToText(VkResult Result) noexcept;
//
//	VkInstance instance;
//
//	VkSurfaceKHR surface;
//
//	VkSwapchainKHR swapChain;
//
//	VkPhysicalDevice selectedPhysicalDevice;
//
//	int familyIndex = -1;
//
//	VkDevice device;
//
//	VkSemaphore acquireSemaphore;
//
//	VkSemaphore renderSemaphore;
//
//	VkQueue queue;
//
//	VkFence renderFence;
//
//	VkQueue renderQueue;
//#ifdef MR_DEBUG
//	VkDebugUtilsMessengerEXT dbman;
//#endif // MR_DEBUG
//};

