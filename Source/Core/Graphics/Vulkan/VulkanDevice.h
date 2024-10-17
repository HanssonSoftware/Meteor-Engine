///* Copyright 2020-2024, Phoenix Games. All rights reserved. */
//#pragma once
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
//
//// Add our supported layers
//static constexpr const char* RequiredMinimumLayers[] =
//{
//	"VK_LAYER_KHRONOS_validation"
//	//"VK_LAYER_AMD_switchable_graphics"
//};
//
//static constexpr unsigned int RequiredMinimumLayersCount = std::size(RequiredMinimumLayers);
//
//// Add our supported device extensions
//static constexpr const char* RequiredMinimumDeviceExtensions[] =
//{
//	VK_KHR_SWAPCHAIN_EXTENSION_NAME
//};
//
//static constexpr unsigned int RequiredMinimumDeviceExtensionsCount = std::size(RequiredMinimumDeviceExtensions);
//
//// Add our supported instance extensions
//static constexpr const char* RequiredMinimumInstanceExtensions[] =
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
//static constexpr unsigned int RequiredMinimumInstanceExtensionsCount = std::size(RequiredMinimumInstanceExtensions);
//
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
//	// Inherited via IGraphicsDevice
//	virtual void init() override;
//
//	virtual void cleanUp() override;
//
//	virtual void render() override;
//
//	virtual const std::wstring getRendererSignatature() const;
//private:
//	inline void createDevice();
//
//	inline void createSwapChain();
//
//	inline void createInstance();
//
//	inline void createSurface();
//
//	inline void querryCapabilities();
//
//	inline void createDebugMessenger();
//
//	inline void createSemaphores();
//
//	inline void createCommandPool();
//
//	inline void createFences();
//
//	/** "Converts" the result code to text, which can be pasted to log or assertion error message.*/
//	constexpr const wchar_t* evaluateResultToText(VkResult Result);
//
//	VkInstance instance;
//
//	VkSwapchainKHR swapChain;
//
//	VkSemaphore preRenderSemaphore;
//
//	VkSemaphore renderFinishedSemaphore;
//
//	VkFence preRenderWaitFence;
//
//	/** Serves as a logical interface between the real one (VkDevice). */
//	VkPhysicalDevice selectedDevice;
//	int selectedDeviceIndex = -1;
//
//	VkSurfaceKHR surface;
//
//	VkDevice physicalDevice;
//
//	uint32 swapChainImagesQuantity = 0;
//
//	VkImage swapChainImages[3];
//
//	VkQueue renderQueue;
//
//	VkCommandPool commandPool;
//
//	std::vector<VkFramebuffer> swapChainBuffers;
//
//#ifdef MR_DEBUG
//	VkDebugUtilsMessengerEXT dbman;
//#endif // MR_DEBUG
//};
//
