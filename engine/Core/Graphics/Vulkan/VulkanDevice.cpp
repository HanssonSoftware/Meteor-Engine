/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "VulkanDevice.h"
//#include <Application/Application.h>
//#include <Common/MemoryManager.h>
//#include <Log/Exception.h>
//#include <Window/WindowManager.h>
//#include <thread>
//#include <mutex>


// TODO: ArraySize define: sizeof(InputArr) / sizeof(InputArr[0])
//
//
//static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
//	VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
//	VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
//	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
//	void* pUserData)
//{
//	const size_t requiredSize = strlen(pCallbackData->pMessage);
//
//	wchar_t* buffer = (wchar_t*)mrmalloc((requiredSize + 1) * sizeof(wchar_t));
//	mbstowcs(buffer, pCallbackData->pMessage, requiredSize + 1);
//
//	switch (messageSeverity)
//	{
//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
//		{
//			MR_LOG(LogVulkan, Warn, TEXT("%s"), buffer);
//		}
//		break;
//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
//		{
//			MR_LOG(LogVulkan, Error, TEXT("%s"), buffer);
//		}
//		break;
//	default:
//		break;
//	}
//
//	mrfree(buffer);
//	return false;
//}
//
//
// IVulkanDevice::IVulkanDevice()
// {
// }
//
// IVulkanDevice::~IVulkanDevice()
// {
// }
//
// bool IVulkanDevice::Init()
//{
//	initInstance();
//
//	createDebugMessenger();
//
//	selectDevice();
//
//	initDevices();
//
//	createSurface();
//
//	createSwapChain();
//
//	createSemaphoresAndFences();
//
//	bIsDriverInitialized = true;
//	setDeviceReadyState(true);
//	return true;
//}
//
//void IVulkanDevice::cleanUp()
//{
//	vkDestroySwapchainKHR(device, swapChain, 0);
//
//	vkDestroyFence(device, renderFence, 0);
//	vkDestroySemaphore(device, renderSemaphore, 0);
//	vkDestroySemaphore(device, acquireSemaphore, 0);
//
//	vkDestroySurfaceKHR(instance, surface, 0);
//
//	vkDestroyDevice(device, 0);
//
//#ifdef MR_DEBUG
//	PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT =
//		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
//
//	if (vkDestroyDebugUtilsMessengerEXT)
//		vkDestroyDebugUtilsMessengerEXT(instance, dbman, NULL);
//#endif // MR_DEBUG
//
//	vkDestroyInstance(instance, NULL);
//
//	MR_LOG(LogVulkan, Log, TEXT("Vulkan successfuly destructed!"));
//}
//
//bool IVulkanDevice::Render(float deltaTime)
//{
//	if (!(IGraphicsDevice::Render(deltaTime)))
//		return false;
//
//	VkQueue a;
//	uint32 getIndex = 0;
//	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, acquireSemaphore, 0, &getIndex);
//
//	vkGetDeviceQueue(device, familyIndex, 0, &a);
//
//
//	//vkQueueSubmit()
//	//vkQueuePresentKHR(a, )
//
//	vkWaitForFences(device, 1, &renderFence, 1, UINT64_MAX);
//	vkResetFences(device, 1, &renderFence);
//
//	return true;
//}
//
//const String IVulkanDevice::getRendererSignatature() const
//{
//	return L"Vulkan";
//}
//
//inline void IVulkanDevice::initInstance()
//{
//	VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
//	appInfo.apiVersion = VK_API_VERSION_1_3;
//
//	VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
//	createInfo.pApplicationInfo = &appInfo;
//	createInfo.enabledExtensionCount = requiredMinimalInstanceExtensionsCount;
//	createInfo.enabledLayerCount = requiredMinimalLayersCount;
//	createInfo.ppEnabledExtensionNames = requiredMinimalInstanceExtensions;
//	createInfo.ppEnabledLayerNames = requiredMinimalLayers;
//
//	VkResult Result = vkCreateInstance(&createInfo, 0, &instance);
//	if (Result != VK_SUCCESS)
//	{
//		THROW_EXCEPTION("vkCreateInstance failed with: %s", evaluateResultToText(Result));
//	}
//}
//
//#ifdef MR_DEBUG
//void IVulkanDevice::createDebugMessenger()
//{
//	VkDebugUtilsMessengerCreateInfoEXT dbInfoE = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
//	dbInfoE.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//	dbInfoE.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
//	dbInfoE.pfnUserCallback = debugCallback;
//
//	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT =
//		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
//
//	if (vkCreateDebugUtilsMessengerEXT) {
//		vkCreateDebugUtilsMessengerEXT(instance, &dbInfoE, NULL, &dbman);
//		return;
//	}
//
//	MR_LOG(LogVulkan, Error, TEXT("Failed to Initialize Vulkan debug messenger!"));
//}
//#endif // DEBUG
//
//inline void IVulkanDevice::selectDevice()
//{
//	VkPhysicalDevice devices[8];
//	uint32 devicesCount = 0;
//
//	vkEnumeratePhysicalDevices(instance, &devicesCount, 0);
//	vkEnumeratePhysicalDevices(instance, &devicesCount, devices);
//
//	for (uint32 i = 0; i < devicesCount; i++)
//	{
//		VkPhysicalDeviceProperties deviceProps;
//		vkGetPhysicalDeviceProperties(devices[i], &deviceProps);
//
//		VkPhysicalDeviceFeatures deviceFeatures;
//		vkGetPhysicalDeviceFeatures(devices[i], &deviceFeatures);
//		
//		if (deviceFeatures.geometryShader == 0)
//			continue;
//
//		if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
//		{
//			MR_LOG(LogVulkan, Warn, TEXT("Vulkan is Being Initialized with an Integrated GPU!"));
//		}
//		else if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
//		{
//			MR_LOG(LogVulkan, Warn, TEXT("Vulkan is Being Initialized with a CPU!"));
//		}
//
//		uint32 familyPropertiesCount = 0;
//		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &familyPropertiesCount, 0);
//		std::vector<VkQueueFamilyProperties> familyProperties(familyPropertiesCount);
//		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &familyPropertiesCount, familyProperties.data());
//
//		for (uint32 j = 0; j < familyPropertiesCount; j++)
//		{
//			if (familyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
//			{
//				selectedPhysicalDevice = devices[i];
//				familyIndex = j;
//
//
//				//uint32 instanceExtensionsCount = 0;
//				//uint32 deviceExtensionsCount = 0;
//				//vkEnumerateInstanceExtensionProperties(0, &instanceExtensionsCount, 0);
//				//std::vector<VkExtensionProperties> instanceExtensions(instanceExtensionsCount);
//				//vkEnumerateInstanceExtensionProperties(0, &instanceExtensionsCount, instanceExtensions.data());
//
//				//vkEnumerateDeviceExtensionProperties(selectedPhysicalDevice, 0, &deviceExtensionsCount, 0);
//				//std::vector<VkExtensionProperties> deviceExtensions(deviceExtensionsCount);
//				//vkEnumerateDeviceExtensionProperties(selectedPhysicalDevice, 0, &deviceExtensionsCount, deviceExtensions.data());
//
//				return;
//			}
//		}
//	}
//
//	MR_LOG(LogVulkan, Fatal, TEXT("Unable to found any capable gpu(s)!"));
//}
//
//inline void IVulkanDevice::initDevices()
//{
//	if (!selectedPhysicalDevice)
//	{
//		MR_LOG(LogVulkan, Fatal, TEXT("selectedPhysicalDevice is invalid, or null!"));
//	}
//
//	static constexpr const float Priority = 1.f;
//	VkDeviceQueueCreateInfo queueInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
//	queueInfo.queueCount = 1;
//	queueInfo.queueFamilyIndex = familyIndex;
//	queueInfo.pQueuePriorities = &Priority;
//
//	VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
//	createInfo.enabledExtensionCount = requiredMinimalDeviceExtensionsCount;
//	createInfo.ppEnabledExtensionNames = requiredMinimalDeviceExtensions;
//	createInfo.enabledLayerCount = requiredMinimalLayersCount;
//	createInfo.ppEnabledLayerNames = requiredMinimalLayers;
//	createInfo.pQueueCreateInfos = &queueInfo;
//	createInfo.queueCreateInfoCount = 1;
//
//
//	VkResult Result = vkCreateDevice(selectedPhysicalDevice, &createInfo, 0, &device);
//	if (Result != VK_SUCCESS)
//	{
//		THROW_EXCEPTION("vkCreateDevice failed with: %s", evaluateResultToText(Result));
//	}
//
//	vkGetDeviceQueue(device, familyIndex, 0, &queue);
//}
//
//inline void IVulkanDevice::createSurface()
//{
//	VkResult Result = VK_ERROR_SURFACE_LOST_KHR;
//#ifdef _WIN32
//	VkWin32SurfaceCreateInfoKHR surfaceInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
//	surfaceInfo.hinstance = GetModuleHandle(NULL);
//	surfaceInfo.hwnd = (HWND)Application::Get()->getWindowManager()->getFirstWindow()->getWindowHandle();
//
//	Result = vkCreateWin32SurfaceKHR(instance, &surfaceInfo, 0, &surface);
//#endif // _WIN32
//
//	if (Result != VK_SUCCESS)
//	{
//		THROW_EXCEPTION("Failed to Create Vulkan surface! %s", evaluateResultToText(Result));
//	}
//}
//
//inline void IVulkanDevice::createSwapChain()
//{
//	VkSurfaceCapabilitiesKHR capabilities;
//	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(selectedPhysicalDevice, surface, &capabilities);
//
//	VkSurfaceFormatKHR selectedSurfaceFormat = {};
//	uint32 surfaceFormatCount = 0;
//	vkGetPhysicalDeviceSurfaceFormatsKHR(selectedPhysicalDevice, surface, &surfaceFormatCount, 0);
//
//	std::vector<VkSurfaceFormatKHR> formats(surfaceFormatCount);
//	vkGetPhysicalDeviceSurfaceFormatsKHR(selectedPhysicalDevice, surface, &surfaceFormatCount, formats.data());
//
//	for (const auto& indexedFormat : formats)
//	{
//		if (indexedFormat.format == VK_FORMAT_B8G8R8A8_SRGB && indexedFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
//			selectedSurfaceFormat = indexedFormat;
//	}	
//
//	VkSwapchainCreateInfoKHR swapChainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
//	swapChainInfo.surface = surface;
//	swapChainInfo.clipped = 1;
//	swapChainInfo.minImageCount = 2;
//	swapChainInfo.imageFormat = selectedSurfaceFormat.format;
//	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//	swapChainInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
//	swapChainInfo.imageExtent = capabilities.currentExtent;
//	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
//	swapChainInfo.imageArrayLayers = 1;
//	swapChainInfo.preTransform = capabilities.currentTransform;
//	swapChainInfo.minImageCount = capabilities.minImageCount;
//	swapChainInfo.queueFamilyIndexCount = 1;
//	swapChainInfo.imageColorSpace = selectedSurfaceFormat.colorSpace;
//
//	VkResult Result = vkCreateSwapchainKHR(device, &swapChainInfo, 0, &swapChain);
//	if (Result != VK_SUCCESS)
//	{
//		THROW_EXCEPTION("vkCreateSwapchainKHR failed with: %s", evaluateResultToText(Result));
//	}
//}
//
//inline void IVulkanDevice::createSemaphoresAndFences()
//{
//	VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
//	semaphoreInfo.flags = VK_SEMAPHORE_TYPE_BINARY;
//
//	VkResult Result = vkCreateSemaphore(device, &semaphoreInfo, 0, &acquireSemaphore);
//	if (Result != VK_SUCCESS)
//	{
//		THROW_EXCEPTION("vkCreateSemaphore (1) failed with: %s", evaluateResultToText(Result));
//	}	
//	
//	Result = vkCreateSemaphore(device, &semaphoreInfo, 0, &renderSemaphore);
//	if (Result != VK_SUCCESS)
//	{
//		THROW_EXCEPTION("vkCreateSemaphore (2) failed with: %s", evaluateResultToText(Result));
//	}
//
//	VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
//	fenceInfo.flags = VK_EVENT_CREATE_DEVICE_ONLY_BIT;
//
//	Result = vkCreateFence(device, &fenceInfo, 0, &renderFence);
//	if (Result != VK_SUCCESS)
//	{
//		THROW_EXCEPTION("vkCreateFence failed with: %s", evaluateResultToText(Result));
//	}
//}
//
//const String IVulkanDevice::evaluateResultToText(VkResult Result) noexcept
//{
//	switch (Result)
//	{
//	case VK_SUCCESS:
//		return L"VK_SUCCESS";
//	case VK_NOT_READY:
//		return L"VK_NOT_READY";
//	case VK_TIMEOUT:
//		return L"VK_TIMEOUT";
//	case VK_EVENT_SET:
//		return L"VK_EVENT_SET";
//	case VK_EVENT_RESET:
//		return L"VK_EVENT_RESET";
//	case VK_INCOMPLETE:
//		return L"VK_INCOMPLETE";
//	case VK_ERROR_OUT_OF_HOST_MEMORY:
//		return L"VK_ERROR_OUT_OF_HOST_MEMORY";
//	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
//		return L"VK_ERROR_OUT_OF_DEVICE_MEMORY";
//	case VK_ERROR_INITIALIZATION_FAILED:
//		return L"VK_ERROR_INITIALIZATION_FAILED";
//	case VK_ERROR_DEVICE_LOST:
//		return L"VK_ERROR_DEVICE_LOST";
//	case VK_ERROR_MEMORY_MAP_FAILED:
//		return L"VK_ERROR_MEMORY_MAP_FAILED";
//	case VK_ERROR_LAYER_NOT_PRESENT:
//		return L"VK_ERROR_LAYER_NOT_PRESENT";
//	case VK_ERROR_EXTENSION_NOT_PRESENT:
//		return L"VK_ERROR_EXTENSION_NOT_PRESENT";
//	case VK_ERROR_FEATURE_NOT_PRESENT:
//		return L"VK_ERROR_FEATURE_NOT_PRESENT";
//	case VK_ERROR_INCOMPATIBLE_DRIVER:
//		return L"VK_ERROR_INCOMPATIBLE_DRIVER";
//	case VK_ERROR_TOO_MANY_OBJECTS:
//		return L"VK_ERROR_TOO_MANY_OBJECTS";
//	case VK_ERROR_FORMAT_NOT_SUPPORTED:
//		return L"VK_ERROR_FORMAT_NOT_SUPPORTED";
//	case VK_ERROR_FRAGMENTED_POOL:
//		return L"VK_ERROR_FRAGMENTED_POOL";
//	case VK_ERROR_UNKNOWN:
//		return L"VK_ERROR_UNKNOWN";
//	case VK_ERROR_OUT_OF_POOL_MEMORY:
//		return L"VK_ERROR_OUT_OF_POOL_MEMORY";
//	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
//		return L"VK_ERROR_INVALID_EXTERNAL_HANDLE";
//	case VK_ERROR_FRAGMENTATION:
//		return L"VK_ERROR_FRAGMENTATION";
//	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
//		return L"VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
//	case VK_PIPELINE_COMPILE_REQUIRED:
//		return L"VK_PIPELINE_COMPILE_REQUIRED";
//	case VK_ERROR_SURFACE_LOST_KHR:
//		return L"VK_ERROR_SURFACE_LOST_KHR";
//	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
//		return L"VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
//	case VK_SUBOPTIMAL_KHR:
//		return L"VK_SUBOPTIMAL_KHR";
//	case VK_ERROR_OUT_OF_DATE_KHR:
//		return L"VK_ERROR_OUT_OF_DATE_KHR";
//	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
//		return L"VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
//	case VK_ERROR_VALIDATION_FAILED_EXT:
//		return L"VK_ERROR_VALIDATION_FAILED_EXT";
//	case VK_ERROR_INVALID_SHADER_NV:
//		return L"VK_ERROR_INVALID_SHADER_NV";
//	case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
//		return L"VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
//	case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
//		return L"VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
//	case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
//		return L"VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
//	case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
//		return L"VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
//	case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
//		return L"VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
//	case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
//		return L"VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
//	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
//		return L"VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
//	case VK_ERROR_NOT_PERMITTED_KHR:
//		return L"VK_ERROR_NOT_PERMITTED_KHR";
//	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
//		return L"VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
//	case VK_THREAD_IDLE_KHR:
//		return L"VK_THREAD_IDLE_KHR";
//	case VK_THREAD_DONE_KHR:
//		return L"VK_THREAD_DONE_KHR";
//	case VK_OPERATION_DEFERRED_KHR:
//		return L"VK_OPERATION_DEFERRED_KHR";
//	case VK_OPERATION_NOT_DEFERRED_KHR:
//		return L"VK_OPERATION_NOT_DEFERRED_KHR";
//	case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
//		return L"VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
//	case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
//		return L"VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
//	case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
//		return L"VK_INCOMPATIBLE_SHADER_BINARY_EXT";
//	case VK_RESULT_MAX_ENUM:
//		return L"VK_RESULT_MAX_ENUM";
//	default:
//		break;
//	}
//
//	return L"???";
//}
