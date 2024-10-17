///* Copyright 2020-2024, Phoenix Games.All rights reserved. */
//#include "VulkanDevice.h"
//#include <Application/Application.h>
//
//
////STATIC_LOG_ADDCATEGORY(Windows);
//
//// TODO: ArraySize define: sizeof(InputArr) / sizeof(InputArr[0])
//
//
//static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
//	VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
//	VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
//	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
//	void* pUserData)
//{
//	switch (messageSeverity)
//	{
//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
//		break;
//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
//		break;
//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
//		{
//			wchar_t Temp[2048];
//			size_t conv = 0;
//			auto s = mbstowcs_s(&conv, Temp, pCallbackData->pMessage, strlen(pCallbackData->pMessage));
//			MR_LOG(LogVulkan, Warn, TEXT("%s"), Temp);
//		}
//		break;
//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
//		{
//			wchar_t Temp[2048];
//			size_t conv = 0;
//			mbstowcs_s(&conv, Temp, pCallbackData->pMessage, strlen(pCallbackData->pMessage));
//			MR_LOG(LogVulkan, Error, TEXT("%s"), Temp);
//		}
//		break;
//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
//		break;
//	default:
//		break;
//	}
//
//	return false;
//}
//
//
//void IVulkanDevice::init()
//{
//	createInstance();
//#ifdef MR_DEBUG
//	createDebugMessenger();
//#endif // MR_DEBUG
//	querryCapabilities();
//
//	createDevice();
//
//	createSurface();
//
//	createSwapChain();
//
//	createCommandPool();
//
//	createSemaphores();
//
//	createFences();
//
//	setDriverReadyState(true);
//	MR_LOG(LogVulkan, Log, TEXT("Vulkan is successfuly initialized!"));
//}
//
//void IVulkanDevice::cleanUp()
//{
//
//	vkDestroySemaphore(physicalDevice, preRenderSemaphore, NULL);
//	vkDestroySemaphore(physicalDevice, renderFinishedSemaphore, NULL);
//
//	vkDestroyFence(physicalDevice, preRenderWaitFence, NULL);
//	
//	vkDestroyCommandPool(physicalDevice, commandPool, NULL);
//
//	vkDestroySwapchainKHR(physicalDevice, swapChain, NULL);
//
//	vkDestroySurfaceKHR(instance, surface, NULL);
//
//	vkDestroyDevice(physicalDevice, NULL);
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
//void IVulkanDevice::render()
//{
//	IGraphicsDevice::render();
//	//uint32 imageIndex = 0;
//	//auto ResvkAc = vkAcquireNextImageKHR(physicalDevice, swapChain, INT_MAX, preRenderSemaphore, NULL, &imageIndex);
//
//	//VkCommandBuffer tempBuffer;
//	//VkCommandBufferAllocateInfo cmdbufferInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
//	//cmdbufferInfo.commandBufferCount = 1;
//	//cmdbufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//	//cmdbufferInfo.commandPool = commandPool;
//	//auto ResvkAl = vkAllocateCommandBuffers(physicalDevice, &cmdbufferInfo, &tempBuffer);
//
//	//VkCommandBufferBeginInfo cmdbufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
//	//cmdbufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//
//	//auto ResvkBe = vkBeginCommandBuffer(tempBuffer, &cmdbufferBeginInfo);
//
//	//VkClearColorValue color = { 0.13f, 0.14f, 0.17f, 1.f};
//	//VkImageSubresourceRange range = {};
//	//range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//	//range.layerCount = 1;
//	//range.levelCount = 1;
//
//	//vkCmdClearColorImage(tempBuffer, swapChainImages[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1, &range);
//
//	//auto ResvkEn = vkEndCommandBuffer(tempBuffer);
//
//	//VkPipelineStageFlags pplFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//
//	//VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
//	//submitInfo.commandBufferCount = 1;
//	//submitInfo.pWaitDstStageMask = &pplFlags;
//	//submitInfo.pCommandBuffers = &tempBuffer;
//	//submitInfo.pSignalSemaphores = &preRenderSemaphore;
//	//submitInfo.signalSemaphoreCount = 1;
//	//submitInfo.pWaitSemaphores = &renderFinishedSemaphore;
//	//submitInfo.waitSemaphoreCount = 1;
//
//	//auto ResvkQu = vkQueueSubmit(renderQueue, 1, &submitInfo, 0);
//
//
//	//VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
//	//presentInfo.pSwapchains = &swapChain;
//	//presentInfo.swapchainCount = 1;
//	//presentInfo.pImageIndices = &imageIndex;
//	//presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
//	//presentInfo.waitSemaphoreCount = 1;
//
//	//auto ResvkQue = vkQueuePresentKHR(renderQueue, &presentInfo);
//
//	//auto ResvkWa = vkWaitForFences(physicalDevice, 1, &preRenderWaitFence, VK_TRUE, INT_MAX);
//
//	//auto ResvkDe = vkDeviceWaitIdle(physicalDevice);
//
//	//vkFreeCommandBuffers(physicalDevice, commandPool, 1, &tempBuffer);
//}
//
//const std::wstring IVulkanDevice::getRendererSignatature() const
//{
//	return L"Vulkan";
//}
//
//void IVulkanDevice::createSwapChain()
//{
//	VkSurfaceCapabilitiesKHR sfCapabilitiesInfo = { VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR };
//	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(selectedDevice, surface, &sfCapabilitiesInfo);
//
//	VkSwapchainCreateInfoKHR scInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
//	scInfo.surface = surface;
//	scInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
//	scInfo.minImageCount = 2;
//	scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
//	scInfo.preTransform = sfCapabilitiesInfo.currentTransform;
//	scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//	scInfo.imageArrayLayers = 1;
//	scInfo.presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
//	scInfo.imageExtent = sfCapabilitiesInfo.currentExtent;
//	scInfo.clipped = VK_TRUE;
//
//	VkResult Result = vkCreateSwapchainKHR(physicalDevice, &scInfo, NULL, &swapChain);
//	MR_ASSERT(Result == VK_SUCCESS, TEXT("vkCreateSwapchainKHR failed with: %s"), evaluateResultToText(Result));
//
//	vkGetSwapchainImagesKHR(physicalDevice, swapChain, &swapChainImagesQuantity, NULL);
//	vkGetSwapchainImagesKHR(physicalDevice, swapChain, &swapChainImagesQuantity, swapChainImages);
//}
//
//void IVulkanDevice::createDevice()
//{
//	VkDeviceQueueCreateInfo dvQueueInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
//	dvQueueInfo.queueCount = 1;
//
//	constexpr const float priority = 1.f;
//	dvQueueInfo.pQueuePriorities = &priority;
//	dvQueueInfo.queueFamilyIndex = selectedDeviceIndex;
//
//	VkDeviceCreateInfo dvCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
//	dvCreateInfo.queueCreateInfoCount = 1;
//	dvCreateInfo.pQueueCreateInfos = &dvQueueInfo;
//	dvCreateInfo.ppEnabledExtensionNames = RequiredMinimumDeviceExtensions;
//	dvCreateInfo.enabledExtensionCount = RequiredMinimumDeviceExtensionsCount;
//
//	VkResult Result = vkCreateDevice(selectedDevice, &dvCreateInfo, NULL, &physicalDevice);
//	MR_ASSERT(Result == VK_SUCCESS, TEXT("vkCreateDevice failed with: %s"), evaluateResultToText(Result));
//
//	vkGetDeviceQueue(physicalDevice, selectedDeviceIndex, 0, &renderQueue);
//}
//
//void IVulkanDevice::createInstance()
//{
//	VkInstanceCreateInfo inCreateInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
//	inCreateInfo.ppEnabledExtensionNames = RequiredMinimumInstanceExtensions;
//	inCreateInfo.enabledExtensionCount = RequiredMinimumInstanceExtensionsCount;
//
//	inCreateInfo.ppEnabledLayerNames = RequiredMinimumLayers;
//	inCreateInfo.enabledLayerCount = RequiredMinimumLayersCount;
//
//	VkResult Result = vkCreateInstance(&inCreateInfo, NULL, &instance);
//	MR_ASSERT(Result == VK_SUCCESS, TEXT("vkCreateInstance failed with: %s"), evaluateResultToText(Result));
//}
//
//inline void IVulkanDevice::createSurface()
//{
//	MR_ASSERT(instance, TEXT("VkInstance is invalid! Check for more details."));
//#ifdef _WIN32
//	VkWin32SurfaceCreateInfoKHR sfInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
//	sfInfo.hinstance = GetModuleHandle(NULL);
//	sfInfo.hwnd = Application::Get().getWindowHandle();
//	
//	VkResult Result = vkCreateWin32SurfaceKHR(instance, &sfInfo, NULL, &surface);
//	MR_ASSERT(Result == VK_SUCCESS, TEXT("vkCreateWin32SurfaceKHR failed with: %s"), evaluateResultToText(Result));
//#endif // _WIN32
//}
//
//void IVulkanDevice::querryCapabilities()
//{
//	uint32 physicalDevicesQuantity = 0;
//	vkEnumeratePhysicalDevices(instance, &physicalDevicesQuantity, NULL);
//
//	std::vector<VkPhysicalDevice> devices(physicalDevicesQuantity);
//	vkEnumeratePhysicalDevices(instance, &physicalDevicesQuantity, devices.data());
//
//	for (uint32 i = 0; i < physicalDevicesQuantity; i++)
//	{
//		VkPhysicalDevice indexedDevice = devices[i];
//
//		//uint32 devicePropertyQuantity = 0;
//		//vkEnumerateDeviceExtensionProperties(indexedDevice, NULL, &devicePropertyQuantity, NULL);
//
//		//std::vector<VkExtensionProperties> extensions(devicePropertyQuantity);
//		//vkEnumerateDeviceExtensionProperties(indexedDevice, NULL, &devicePropertyQuantity, extensions.data());
//
//		uint32 deviceQueueFamilyQuantity = 0;
//		vkGetPhysicalDeviceQueueFamilyProperties(indexedDevice, &deviceQueueFamilyQuantity, NULL);
//
//		std::vector<VkQueueFamilyProperties> queues(deviceQueueFamilyQuantity);
//		vkGetPhysicalDeviceQueueFamilyProperties(indexedDevice, &deviceQueueFamilyQuantity, queues.data());
//
//		for (uint32 b = 0; b < deviceQueueFamilyQuantity; b++)
//		{
//			VkQueueFamilyProperties property = queues[b];
//
//			if (property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//			{
//				selectedDeviceIndex = b;
//				selectedDevice = indexedDevice;
//				return;
//			}
//		}
//	}
//
//	MR_LOG(LogVulkan, Warn, TEXT("No suitable device found!"));
//}
//
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
//	MR_LOG(LogVulkan, Error, TEXT("Failed to initialize Vulkan debug messenger!"));
//}
//
//inline void IVulkanDevice::createSemaphores()
//{
//	VkSemaphoreCreateInfo smpInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
//	smpInfo.flags = VK_SEMAPHORE_TYPE_BINARY;
//
//	VkResult Result = vkCreateSemaphore(physicalDevice, &smpInfo, NULL, &preRenderSemaphore);
//	VkResult ResultA = vkCreateSemaphore(physicalDevice, &smpInfo, NULL, &renderFinishedSemaphore);
//	MR_ASSERT(Result == VK_SUCCESS, TEXT("vkCreateSemaphore failed with: %s"), evaluateResultToText(Result));
//	MR_ASSERT(ResultA == VK_SUCCESS, TEXT("vkCreateSemaphore failed with: %s"), evaluateResultToText(Result));
//}
//
//inline void IVulkanDevice::createCommandPool()
//{
//	VkCommandPoolCreateInfo cmdPoolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
//	cmdPoolInfo.queueFamilyIndex = selectedDeviceIndex;
//
//	VkResult Result = vkCreateCommandPool(physicalDevice, &cmdPoolInfo, NULL, &commandPool);
//	MR_ASSERT(Result == VK_SUCCESS, TEXT("vkCreateCommandPool failed with: %s"), evaluateResultToText(Result));
//}
//
//inline void IVulkanDevice::createFences()
//{
//	VkFenceCreateInfo fncInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
//
//	VkResult Result = vkCreateFence(physicalDevice, &fncInfo, NULL, &preRenderWaitFence);
//	MR_ASSERT(Result == VK_SUCCESS, TEXT("vkCreateFence failed with: %s"), evaluateResultToText(Result));
//}
//
//constexpr const wchar_t* IVulkanDevice::evaluateResultToText(VkResult Result)
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
