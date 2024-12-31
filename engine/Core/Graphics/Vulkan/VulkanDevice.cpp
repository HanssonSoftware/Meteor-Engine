/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "VulkanDevice.h"
#include <Application/Application.h>
#include <Common/MemoryManager.h>
#include <Log/Exception.h>
#include <Window/WindowManager.h>
#include <thread>
#include <mutex>


// TODO: ArraySize define: sizeof(InputArr) / sizeof(InputArr[0])


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	const size_t requiredSize = strlen(pCallbackData->pMessage);

	wchar_t* buffer = (wchar_t*)mrmalloc((requiredSize + 1) * sizeof(wchar_t));
	mbstowcs(buffer, pCallbackData->pMessage, requiredSize + 1);

	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		{
			MR_LOG(LogVulkan, Warn, TEXT("%s"), buffer);
		}
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		{
			MR_LOG(LogVulkan, Error, TEXT("%s"), buffer);
		}
		break;
	default:
		break;
	}

	mrfree(buffer);
	return false;
}


 IVulkanDevice::IVulkanDevice()
 {
 }

 IVulkanDevice::~IVulkanDevice()
 {
 }

 bool IVulkanDevice::Init()
{
	initInstance();

	createDebugMessenger();

	createSurface();

	selectDevice();

	initDevices();

	createSwapChain();

	createSemaphoresAndFences();

	createCommandBuffer();

	bIsDriverInitialized = true;
	setDeviceReadyState(GRAPHICS_ENGINE_STATE_RUNNING);
	return true;
}

void IVulkanDevice::cleanUp()
{
	setDeviceReadyState(GRAPHICS_ENGINE_STATE_SHUTDOWN);
	vkDestroySwapchainKHR(device, swapChain, 0);

	vkDestroyFence(device, renderFence, 0);
	vkDestroySemaphore(device, renderSemaphore, 0);
	vkDestroySemaphore(device, acquireSemaphore, 0);

	vkDestroySurfaceKHR(instance, surface, 0);

	vkDestroyDevice(device, 0);

#ifdef MR_DEBUG
	PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (vkDestroyDebugUtilsMessengerEXT)
		vkDestroyDebugUtilsMessengerEXT(instance, dbman, NULL);
#endif // MR_DEBUG

	vkDestroyInstance(instance, NULL);

	MR_LOG(LogVulkan, Log, TEXT("Vulkan successfuly destructed!"));
}

std::mutex mtx;
bool IVulkanDevice::Render()
{
	if (!(IGraphicsDevice::Render()))
		return false;

	uint32 getIndex = 0;
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, renderSemaphore, 0, &getIndex);


	VkCommandBuffer cmd;
	VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	allocInfo.commandBufferCount = 1;
	allocInfo.commandPool = commandPool;
	vkAllocateCommandBuffers(device, &allocInfo, &cmd);

	VkCommandBufferBeginInfo bufferBInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	bufferBInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(cmd, &bufferBInfo);

	VkClearColorValue f = { 1.f, 1.f, 1.f, 1.f };
	VkImageSubresourceRange g;
	g.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	g.layerCount = 1;
	g.levelCount = 1;
	g.baseArrayLayer = 1;
	g.baseMipLevel = 1;


	vkCmdClearColorImage(cmd, swapChainImages[getIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, &f, 1, &g);

	vkEndCommandBuffer(cmd);

	constexpr const VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.commandBufferCount = 1;
	submitInfo.pWaitDstStageMask = &waitStage;
	submitInfo.pCommandBuffers = &cmd;
	submitInfo.pSignalSemaphores = &acquireSemaphore;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &renderSemaphore;
	
	VkResult result2 = vkQueueSubmit(queues.graphicsQueue, 1, &submitInfo, renderFence);

	VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.pImageIndices = &getIndex;
	presentInfo.pSwapchains = &swapChain;
	presentInfo.pWaitSemaphores = &acquireSemaphore;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.swapchainCount = 1;


	VkResult result = vkQueuePresentKHR(queues.graphicsQueue, &presentInfo);


	vkWaitForFences(device, 1, &renderFence, 1, UINT64_MAX);
	vkResetFences(device, 1, &renderFence);
	
	vkDeviceWaitIdle(device);
	vkFreeCommandBuffers(device, commandPool, 1, &cmd);

	return true;
}

const String IVulkanDevice::getRendererSignatature() const
{
	return L"Vulkan";
}

inline void IVulkanDevice::initInstance()
{
	VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	appInfo.apiVersion = VK_API_VERSION_1_3;

	static constexpr const char* instanceExtensions[]
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif // _WIN32
#ifdef MR_DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif // MR_DEBUG
	};

	static constexpr unsigned int instanceExtensionsSize = std::size(instanceExtensions);

	VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = instanceExtensionsSize;
	createInfo.enabledLayerCount = requiredMinimalLayersCount;
	createInfo.ppEnabledExtensionNames = instanceExtensions;
	createInfo.ppEnabledLayerNames = requiredMinimalLayers;

	VkResult Result = vkCreateInstance(&createInfo, 0, &instance);
	if (Result != VK_SUCCESS)
	{
		THROW_EXCEPTION("vkCreateInstance failed with: %s", evaluateResultToText(Result));
	}
}

#ifdef MR_DEBUG
void IVulkanDevice::createDebugMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT dbInfoE = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
	dbInfoE.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	dbInfoE.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
	dbInfoE.pfnUserCallback = debugCallback;

	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (vkCreateDebugUtilsMessengerEXT) {
		vkCreateDebugUtilsMessengerEXT(instance, &dbInfoE, NULL, &dbman);
		return;
	}

	MR_LOG(LogVulkan, Error, TEXT("Failed to Initialize Vulkan Debug Messenger!"));
}
#endif // DEBUG

inline void IVulkanDevice::selectDevice()
{
	VkPhysicalDevice devices[8];
	uint32 devicesCount = 0;

	vkEnumeratePhysicalDevices(instance, &devicesCount, 0);
	vkEnumeratePhysicalDevices(instance, &devicesCount, devices);

	for (uint32 i = 0; i < devicesCount; i++)
	{
		VkPhysicalDevice indexedDevice = devices[i];

		VkPhysicalDeviceProperties deviceProps;
		vkGetPhysicalDeviceProperties(indexedDevice, &deviceProps);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(indexedDevice, &deviceFeatures);
		
		if (deviceFeatures.geometryShader == 0 && deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			continue;

		uint32 queueFamilyPropertiesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(indexedDevice, &queueFamilyPropertiesCount, 0);

		std::vector<VkQueueFamilyProperties> familyProperties(queueFamilyPropertiesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(indexedDevice, &queueFamilyPropertiesCount, familyProperties.data());

		for (uint32 j = 0; j < queueFamilyPropertiesCount; j++)
		{
			VkQueueFamilyProperties indexedFamilyProperties = familyProperties[j];

			if (indexedFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				selectedPhysicalDevice = indexedDevice;
				queues.graphicsQueueIndex = j;
			}

			VkBool32 bIsTrue = 0;
			vkGetPhysicalDeviceSurfaceSupportKHR(indexedDevice, j, surface, &bIsTrue);

			if (bIsTrue)
			{
				queues.presentingQueueIndex = j;
				return;
			}
		}
	}

	MR_LOG(LogVulkan, Fatal, TEXT("Unable to found any capable gpu(s)!"));
}

inline void IVulkanDevice::initDevices()
{
	if (!selectedPhysicalDevice)
	{
		MR_LOG(LogVulkan, Fatal, TEXT("selectedPhysicalDevice is invalid, or null!"));
	}

	static constexpr const float Priority = 1.f;
	VkDeviceQueueCreateInfo queueInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	queueInfo.queueCount = 1;
	queueInfo.queueFamilyIndex = queues.graphicsQueueIndex;
	queueInfo.pQueuePriorities = &Priority;

	static constexpr const char* deviceExtensions[]
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	static constexpr unsigned int deviceExtensionsSize = std::size(deviceExtensions);

	VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	createInfo.enabledExtensionCount = deviceExtensionsSize;
	createInfo.ppEnabledExtensionNames = deviceExtensions;
	createInfo.enabledLayerCount = requiredMinimalLayersCount;
	createInfo.ppEnabledLayerNames = requiredMinimalLayers;
	createInfo.pQueueCreateInfos = &queueInfo;
	createInfo.queueCreateInfoCount = 1;


	VkResult Result = vkCreateDevice(selectedPhysicalDevice, &createInfo, 0, &device);
	if (Result != VK_SUCCESS)
	{
		THROW_EXCEPTION("vkCreateDevice failed with: %s", evaluateResultToText(Result));
	}

	if (queues.graphicsQueueIndex != -1)
		vkGetDeviceQueue(device, queues.graphicsQueueIndex, 0, &queues.graphicsQueue);

	if (queues.presentingQueueIndex != -1)
		vkGetDeviceQueue(device, queues.presentingQueueIndex, 0, &queues.presentingQueue);
}

inline void IVulkanDevice::createSurface()
{
	VkResult Result = VK_ERROR_DEVICE_LOST;
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR surfaceInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
	surfaceInfo.hinstance = GetModuleHandle(NULL);
	surfaceInfo.hwnd = (HWND)Application::Get()->getWindowManager()->getFirstWindow()->getWindowHandle();

	Result = vkCreateWin32SurfaceKHR(instance, &surfaceInfo, 0, &surface);
#endif // _WIN32

	if (Result != VK_SUCCESS)
	{
		THROW_EXCEPTION("Failed to Create Vulkan surface! %s", evaluateResultToText(Result));
	}
}

inline void IVulkanDevice::createCommandBuffer()
{
	VkCommandPoolCreateInfo commandPoolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	commandPoolInfo.queueFamilyIndex = queues.graphicsQueueIndex;

	VkResult Result = vkCreateCommandPool(device, &commandPoolInfo, 0,&commandPool);
	if (Result != VK_SUCCESS)
	{
		THROW_EXCEPTION("vkCreateCommandPool Failed with: %s", evaluateResultToText(Result));
	}
}

inline void IVulkanDevice::createSwapChain()
{
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(selectedPhysicalDevice, surface, &capabilities);

	VkSurfaceFormatKHR selectedSurfaceFormat = {};
	uint32 surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(selectedPhysicalDevice, surface, &surfaceFormatCount, 0);

	std::vector<VkSurfaceFormatKHR> formats(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(selectedPhysicalDevice, surface, &surfaceFormatCount, formats.data());

	for (const auto& indexedFormat : formats)
	{
		if (indexedFormat.format == VK_FORMAT_B8G8R8A8_SRGB && indexedFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
			selectedSurfaceFormat = indexedFormat;
	}	

	VkSwapchainCreateInfoKHR swapChainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	swapChainInfo.surface = surface;
	swapChainInfo.clipped = 1;
	swapChainInfo.minImageCount = 2;
	swapChainInfo.imageFormat = selectedSurfaceFormat.format;
	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	swapChainInfo.imageExtent = capabilities.currentExtent;
	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainInfo.imageArrayLayers = 1;
	swapChainInfo.preTransform = capabilities.currentTransform;
	swapChainInfo.minImageCount = capabilities.minImageCount;
	swapChainInfo.queueFamilyIndexCount = 1;
	swapChainInfo.imageColorSpace = selectedSurfaceFormat.colorSpace;

	VkResult Result = vkCreateSwapchainKHR(device, &swapChainInfo, 0, &swapChain);
	if (Result != VK_SUCCESS)
	{
		THROW_EXCEPTION("vkCreateSwapchainKHR failed with: %s", evaluateResultToText(Result));
	}

	uint32 swapChainImagesCount = 0;
	vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, 0);

	swapChainImages.resize(swapChainImagesCount);
	swapChainImageViews.resize(swapChainImagesCount);
	vkGetSwapchainImagesKHR(device, swapChain, &swapChainImagesCount, swapChainImages.data());

	VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
	viewInfo.format = selectedSurfaceFormat.format;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.layerCount = 1;
	for (uint32_t i = 0; i < swapChainImagesCount; i++)
	{
		viewInfo.image = swapChainImages[i];
		vkCreateImageView(device, &viewInfo, 0, &swapChainImageViews[i]);
	}
}

inline void IVulkanDevice::createSemaphoresAndFences()
{
	VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
	semaphoreInfo.flags = VK_SEMAPHORE_TYPE_BINARY;

	VkResult Result = vkCreateSemaphore(device, &semaphoreInfo, 0, &acquireSemaphore);
	if (Result != VK_SUCCESS)
	{
		THROW_EXCEPTION("vkCreateSemaphore (1) failed with: %s", evaluateResultToText(Result));
	}	
	
	Result = vkCreateSemaphore(device, &semaphoreInfo, 0, &renderSemaphore);
	if (Result != VK_SUCCESS)
	{
		THROW_EXCEPTION("vkCreateSemaphore (2) failed with: %s", evaluateResultToText(Result));
	}

	VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	fenceInfo.flags = 0;

	Result = vkCreateFence(device, &fenceInfo, 0, &renderFence);
	if (Result != VK_SUCCESS)
	{
		THROW_EXCEPTION("vkCreateFence failed with: %s", evaluateResultToText(Result));
	}
}

const String IVulkanDevice::evaluateResultToText(VkResult Result) noexcept
{
	switch (Result)
	{
	case VK_SUCCESS:
		return L"VK_SUCCESS";
	case VK_NOT_READY:
		return L"VK_NOT_READY";
	case VK_TIMEOUT:
		return L"VK_TIMEOUT";
	case VK_EVENT_SET:
		return L"VK_EVENT_SET";
	case VK_EVENT_RESET:
		return L"VK_EVENT_RESET";
	case VK_INCOMPLETE:
		return L"VK_INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return L"VK_ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return L"VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED:
		return L"VK_ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST:
		return L"VK_ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return L"VK_ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return L"VK_ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return L"VK_ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return L"VK_ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return L"VK_ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return L"VK_ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return L"VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_FRAGMENTED_POOL:
		return L"VK_ERROR_FRAGMENTED_POOL";
	case VK_ERROR_UNKNOWN:
		return L"VK_ERROR_UNKNOWN";
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		return L"VK_ERROR_OUT_OF_POOL_MEMORY";
	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
		return L"VK_ERROR_INVALID_EXTERNAL_HANDLE";
	case VK_ERROR_FRAGMENTATION:
		return L"VK_ERROR_FRAGMENTATION";
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
		return L"VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
	case VK_PIPELINE_COMPILE_REQUIRED:
		return L"VK_PIPELINE_COMPILE_REQUIRED";
	case VK_ERROR_SURFACE_LOST_KHR:
		return L"VK_ERROR_SURFACE_LOST_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return L"VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_SUBOPTIMAL_KHR:
		return L"VK_SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return L"VK_ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return L"VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return L"VK_ERROR_VALIDATION_FAILED_EXT";
	case VK_ERROR_INVALID_SHADER_NV:
		return L"VK_ERROR_INVALID_SHADER_NV";
	case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
		return L"VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
		return L"VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
		return L"VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
		return L"VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
		return L"VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
		return L"VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
		return L"VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
	case VK_ERROR_NOT_PERMITTED_KHR:
		return L"VK_ERROR_NOT_PERMITTED_KHR";
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
		return L"VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
	case VK_THREAD_IDLE_KHR:
		return L"VK_THREAD_IDLE_KHR";
	case VK_THREAD_DONE_KHR:
		return L"VK_THREAD_DONE_KHR";
	case VK_OPERATION_DEFERRED_KHR:
		return L"VK_OPERATION_DEFERRED_KHR";
	case VK_OPERATION_NOT_DEFERRED_KHR:
		return L"VK_OPERATION_NOT_DEFERRED_KHR";
	case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
		return L"VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
	case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
		return L"VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
	case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
		return L"VK_INCOMPATIBLE_SHADER_BINARY_EXT";
	case VK_RESULT_MAX_ENUM:
		return L"VK_RESULT_MAX_ENUM";
	default:
		break;
	}

	return L"???";
}
