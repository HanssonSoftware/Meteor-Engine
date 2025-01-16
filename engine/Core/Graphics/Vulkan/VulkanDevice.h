/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "../GraphicsDevice.h"
#include <Log/LogMacros.h>
#include <vector>
#include <vulkan/vulkan.hpp>
#ifdef _WIN32
#include <vulkan/vulkan_win32.h>  
#endif // _WIN32
#include <Types/PlatformDefs.h>

LOG_ADDCATEGORY(Vulkan);

class String;

// Add our supported layers
static constexpr const char* requiredMinimalLayers[] =
{
	"VK_LAYER_KHRONOS_validation"
	//"VK_LAYER_AMD_switchable_graphics"
};

static constexpr unsigned int requiredMinimalLayersCount = std::size(requiredMinimalLayers);

// Add our supported device extensions
static constexpr const char* requiredMinimalInstanceExtensions[] =
{
	"fasz",

#ifdef MR_DEBUG
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif // MR_DEBUG
};

// Add our supported instance extensions
static constexpr const char* requiredMinimalDeviceExtensions[] =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

static constexpr unsigned int requiredMinimalDeviceExtensionsCount = std::size(requiredMinimalDeviceExtensions);
static constexpr unsigned int requiredMinimalInstanceExtensionsCount = std::size(requiredMinimalInstanceExtensions);

typedef struct UsedQueues
{
	VkQueue graphicsQueue;
	int graphicsQueueIndex = -1;	
	
	bool presentingSupported;
	int presentingSupportedFamilyIndex = -1;

	VkQueue transferQueue;
	int transferQueueIndex = -1;

	VkQueue calculationQueue;
	int calculationQueueIndex = -1;
} UsedQueues;


/*
/******		Vulkan graphical interface		******

	* Child class from IGraphicsDevice
	* vkXXX functions must be called twice, first for enumerate (counting), second for actual data to fill.
	* All functions are need to be inlined except inherited members/functions/constants from base.
	
*/
class IVulkanDevice : public IGraphicsDevice
{
public:
	IVulkanDevice();

	virtual ~IVulkanDevice() override;

	// Inherited via IGraphicsDevice
	virtual bool Init() override;

	virtual void cleanUp() override;

	virtual bool Render() override;

	virtual void resizeBuffers(const Vector2<uint32> newSize) {};

	virtual const String getRendererSignatature() const;
private:
	inline void initInstance();

	inline void createDebugMessenger();

	inline void selectDevice();

	inline void initDevices();

	inline void createSurface();

	inline void createCommandBuffer();

	inline void createSwapChain();

	inline void createSemaphoresAndFences();

	inline VkSurfaceFormatKHR selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> formats);

	inline bool checkIfPresentationIsSupported(VkPhysicalDevice virtualDevice, const int Index) const;
	
	/** "Converts" the result code to text, which can be pasted to log or assertion error message.*/
	const String evaluateResultToText(VkResult Result) noexcept;

	VkInstance instance;

	VkSurfaceKHR surface;

	VkSwapchainKHR swapChain;

	VkPhysicalDevice selectedPhysicalDevice;

	int familyIndex = -1;

	VkDevice device;

	std::vector<VkImage> swapChainImages;

	std::vector<VkImageView> swapChainImageViews;

	VkSemaphore acquireSemaphore;

	VkSemaphore renderSemaphore;
	
	UsedQueues queues;

	VkFence renderFence;

	VkCommandBuffer commandBuffer;

	VkCommandPool commandPool;
#ifdef MR_DEBUG
	VkDebugUtilsMessengerEXT dbman;
#endif // MR_DEBUG
};
