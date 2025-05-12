/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "VulkanRHIRegistry.h"
#include "VulkanRHIOutputContext.h"
#include <Logging/LogMacros.h>

#ifdef _WIN64
#include <Windows/Windows.h>
#include <vulkan/vulkan_WIN64.h>
#endif // _WIN64
#include <Core/Application.h>
#include <Platform/PerformanceTimer.h>
#include <Platform/File.h>
#include <Layers/OSLayer.h>


LOG_ADDCATEGORY(Vulkan);

#ifdef MR_DEBUG
static constexpr const bool bIsUsingValidation = true;
#else
static constexpr const bool bIsUsingValidation = false;
#endif

#define APPEND_PRIV(text) \
	L#text

#define LINE_PRIV __LINE__
#define MERGEFUNC(x, y) x##y
#define MERGEFUNC2(x, y) MERGEFUNC(x, y)

#define VK_CHECK(result, func) \
	VkResult MERGEFUNC2(func, LINE_PRIV) = result; bool MERGEFUNC2(boolean, LINE_PRIV) = MERGEFUNC2(func, LINE_PRIV) != VK_SUCCESS; if (MERGEFUNC2(boolean, LINE_PRIV)) { MR_LOG(LogVulkan, Error, "%s returned: %s", #func, evaluateResultToText(MERGEFUNC2(func, LINE_PRIV)).Chr()); } return !MERGEFUNC2(boolean, LINE_PRIV);

int VulkanRHIRegistry::GetMonitorIndex() const
{
	return -1;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) 
{
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		MR_LOG(LogVulkan, Verbose, pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		MR_LOG(LogVulkan, Log, pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		MR_LOG(LogVulkan, Warn, pCallbackData->pMessage);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		MR_LOG(LogVulkan, Error, pCallbackData->pMessage);
		break;
	}

	return /*VK_TRUE*/ 1U;
}

static String privRHIName;
String VulkanRHIRegistry::GetName() const
{
	static constexpr const wchar_t* vkRHIDisplayName = L"Vulkan";

	if (privRHIName.IsEmpty()) privRHIName = vkRHIDisplayName;

	return privRHIName;
}

bool VulkanRHIRegistry::Initialize()
{
	MR_LOG(LogVulkan, Log, "Initializing %s.", GetName().Chr());

	context = new VulkanRHIOutputContext(this);

	bool bError = false;
	bError = !CreateInstance();

	CreateDebugMessenger();

	bError = !CreateSurfaceWin32();

	bError = !CreateQueueSlots();

	bError = !CreateDevice();

	bError = !CreateSwapchain();

	if (bError) CleanUp();
	return true;
}

void VulkanRHIRegistry::CleanUp() const
{
	vkDestroyPipelineLayout(selectedDevice, pipelineLayout, 0);

	vkDestroyShaderModule(selectedDevice, shaderModuleVert, 0);

	vkDestroyShaderModule(selectedDevice, shaderModuleFrag, 0);

	for (VkImageView imageView : swapChainImageViews) 
	{
		vkDestroyImageView(selectedDevice, imageView, 0);
	}

	vkDestroySwapchainKHR(selectedDevice, swapChain, 0);

	vkDestroyDevice(selectedDevice, 0);

	vkDestroySurfaceKHR(instance, surface, 0);

	// VkDebugUtilsMessengerCreateInfoEXT selection
	{
		VkDebugUtilsMessengerCreateInfoEXT dbmonInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		dbmonInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		dbmonInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		dbmonInfo.pfnUserCallback = debugCallback;


		PFN_vkDestroyDebugUtilsMessengerEXT vkDestDebugUtilsEXT =
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

		if (vkDestDebugUtilsEXT)
		{
			vkDestDebugUtilsEXT(instance, dbMon, 0);
		}
	}

	vkDestroyInstance(instance, 0);
}

bool VulkanRHIRegistry::CreateSurfaceWin32()
{
	VkWin32SurfaceCreateInfoKHR win32SurfaceInfo = { VK_STRUCTURE_TYPE_WIN64_SURFACE_CREATE_INFO_KHR };
	if (WindowsWindowManager* windowManager = (WindowsWindowManager*)Application::Get()->GetWindowManager())
	{
		win32SurfaceInfo.hinstance = windowManager->GetInstance();
		win32SurfaceInfo.hwnd = (HWND)windowManager->GetFirstWindow()->GetWindowHandle();
	}
	else
	{
		MR_LOG(LogVulkan, Warn, "Unable to get Window Manager! Using fallback method!");

		win32SurfaceInfo.hinstance = GetModuleHandle(0);
		win32SurfaceInfo.hwnd = GetFocus();
	}

	VK_CHECK(vkCreateWin32SurfaceKHR(instance, &win32SurfaceInfo, 0, &surface), vkCreateWin32SurfaceKHR);
}

bool VulkanRHIRegistry::CreateQueueSlots()
{
	uint32 physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, 0);
	if (physicalDeviceCount == 0)
	{
		MR_LOG(LogVulkan, Error, "Failed to find any physical device!");
		return false;
	}

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	for (VkPhysicalDevice& physicalDevice : physicalDevices)
	{
		uint32 physicalDeviceQueueFamiliesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamiliesCount, 0);
		if (physicalDeviceQueueFamiliesCount == 0)
		{
			MR_LOG(LogVulkan, Warn, "No queue properties found on this device.");
			continue;
		}

		std::vector<VkQueueFamilyProperties> physicalDeviceQueueFamilies(physicalDeviceQueueFamiliesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamiliesCount, physicalDeviceQueueFamilies.data());

		for (uint32 i = 0; i < physicalDeviceQueueFamiliesCount; i++)
		{
			VkQueueFamilyProperties& queueFamilyProps = physicalDeviceQueueFamilies[i];
			if (queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				VkBool32 bIsGood = 0;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &bIsGood);
				if (bIsGood)
				{
					selectedVirtualDevice = physicalDevice;
					selectedQueueIndex = i;
					selectedQueueFamilyCount = (int)queueFamilyProps.queueCount;
					return true;
				}
			}
		}
	}

	return false;
}

bool VulkanRHIRegistry::CreateDevice()
{
	VkDeviceCreateInfo deviceInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };

	static const std::vector<const char*> extensions
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	deviceInfo.enabledExtensionCount = (uint32)extensions.size();
	deviceInfo.ppEnabledExtensionNames = extensions.data();

	VkDeviceQueueCreateInfo queueOne = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	static constexpr float priority = 1.f;

	queueOne.pQueuePriorities = &priority;
	queueOne.queueCount = selectedQueueFamilyCount;
	queueOne.queueFamilyIndex = selectedQueueIndex;

	deviceInfo.pQueueCreateInfos = &queueOne;
	deviceInfo.queueCreateInfoCount = 1;

	VK_CHECK(vkCreateDevice(selectedVirtualDevice, &deviceInfo, 0, &selectedDevice), vkCreateDevice);
}

bool VulkanRHIRegistry::CreateSwapchain()
{
	VkSwapchainCreateInfoKHR swapChainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	swapChainInfo.surface = surface;
	swapChainInfo.minImageCount = 2;
	swapChainInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
	swapChainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	surfaceCapabilities = {};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(selectedVirtualDevice, surface, &surfaceCapabilities);

	swapChainInfo.imageExtent = surfaceCapabilities.maxImageExtent;
	swapChainInfo.imageArrayLayers = 1;
	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapChainInfo.preTransform = surfaceCapabilities.currentTransform;
	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainInfo.clipped = 1;

	VK_CHECK(vkCreateSwapchainKHR(selectedDevice, &swapChainInfo, 0, &swapChain), vkCreateSwapchainKHR);
}

bool VulkanRHIRegistry::CreateImageViews()
{
	uint32 imagesCount = 0;
	vkGetSwapchainImagesKHR(selectedDevice, swapChain, &imagesCount, 0);
	if (imagesCount == 0)
	{
		MR_LOG(LogVulkan, Error, "Failed to get swapchain images!");
		return false;
	}

	swapChainImages.resize(imagesCount);
	swapChainImageViews.resize(imagesCount);

	vkGetSwapchainImagesKHR(selectedDevice, swapChain, &imagesCount, swapChainImages.data());

	static constexpr VkComponentSwizzle COMPONENT_CONSTANT = VK_COMPONENT_SWIZZLE_IDENTITY;
	for (uint32 i = 0; i < imagesCount; i++)
	{
		VkImageViewCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		createInfo.components.r = COMPONENT_CONSTANT;
		createInfo.components.g = COMPONENT_CONSTANT;
		createInfo.components.b = COMPONENT_CONSTANT;
		createInfo.components.a = COMPONENT_CONSTANT;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		vkCreateImageView(selectedDevice, &createInfo, 0, &swapChainImageViews[i]);
	}

	return true;
}

bool VulkanRHIRegistry::initVK()
{
	{
		IFile* shaderVert = CreateFileOperation();
		IFile* shaderFrag = CreateFileOperation();
		FileStatus shaderVertStatus = shaderVert->Open("E:\\meteor\\editor\\vert.spv", OPENRULE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS);
		FileStatus shaderFragStatus = shaderFrag->Open("E:\\meteor\\editor\\frag.spv", OPENRULE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS);

		shaderVert->Read();
		shaderFrag->Read();

		VkShaderModuleCreateInfo createInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
		createInfo.codeSize = shaderVert->GetSize();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderVert->GetBuffer());

		VK_CHECK(vkCreateShaderModule(selectedDevice, &createInfo, 0, &shaderModuleVert), vkCreateShaderModule);
		
		VkShaderModuleCreateInfo createInfoA = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
		createInfoA.codeSize = shaderFrag->GetSize();
		createInfoA.pCode = reinterpret_cast<const uint32_t*>(shaderFrag->GetBuffer());

		VK_CHECK(vkCreateShaderModule(selectedDevice, &createInfoA, 0, &shaderModuleFrag), vkCreateShaderModule);
	}

	{
		VkPipelineShaderStageCreateInfo shaderInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		shaderInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderInfo.module = shaderModuleVert;
		shaderInfo.pName = "main";		
		
		VkPipelineShaderStageCreateInfo shaderInfoA = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		shaderInfoA.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderInfoA.module = shaderModuleFrag;
		shaderInfoA.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { shaderInfo, shaderInfoA };


		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
	
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)surfaceCapabilities.maxImageExtent.width;
		viewport.height = (float)surfaceCapabilities.maxImageExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = surfaceCapabilities.maxImageExtent;

		VkPipelineViewportStateCreateInfo viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.lineWidth = 1.0f;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VK_CHECK(vkCreatePipelineLayout(selectedDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout), vkCreatePipelineLayout);
	}
}

#ifdef MR_DEBUG
static std::vector<const char*> layers
{
	"VK_LAYER_KHRONOS_validation"
};
#endif // MR_DEBUG

bool VulkanRHIRegistry::CreateInstance()
{
	VkInstanceCreateInfo instanceInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };

	static const std::vector<const char*> extensions
	{
#ifdef MR_DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif // MR_DEBUG
		VK_KHR_SURFACE_EXTENSION_NAME,


#ifdef _WIN64
		VK_KHR_WIN64_SURFACE_EXTENSION_NAME
#endif // MR_DEBUG
	};

	VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	appInfo.apiVersion = VK_API_VERSION_1_3;

	instanceInfo.enabledExtensionCount = (uint32)extensions.size();
	instanceInfo.ppEnabledExtensionNames = extensions.data();

#ifdef MR_DEBUG
	instanceInfo.ppEnabledLayerNames = layers.data();
	instanceInfo.enabledLayerCount = (uint32)layers.size();
#else
	instanceInfo.ppEnabledLayerNames = nullptr;
	instanceInfo.enabledLayerCount = 0;
#endif // MR_DEBUG

	instanceInfo.pApplicationInfo = &appInfo;
	VK_CHECK(vkCreateInstance(&instanceInfo, 0, &instance), vkCreateInstance);
}

bool VulkanRHIRegistry::GetInstanceLayerCompatibility()
{
	if (bIsUsingValidation)
	{
		uint32 layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, 0);

		std::vector<VkLayerProperties> validationLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, validationLayers.data());

		bool bFound = false;
		for (const char* layer : layers)
		{
			for (const auto& layerProp : validationLayers)
			{
				if (strcmp(layer, layerProp.layerName) == 0)
				{
					bFound = true;
					break;
				}
			}
		}
	}

	return false;
}

bool VulkanRHIRegistry::CreateDebugMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT dbmonInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
	dbmonInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	dbmonInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	dbmonInfo.pfnUserCallback = debugCallback;


	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsEXT =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (vkCreateDebugUtilsEXT)
	{
		VK_CHECK(vkCreateDebugUtilsEXT(instance, &dbmonInfo, 0, &dbMon), vkCreateDebugUtilsMessengerEXT);
	}
	else
	{
		MR_LOG(LogVulkan, Warn, "Debug utils requested, but not available!");
	}

	return true;
}



const String VulkanRHIRegistry::evaluateResultToText(VkResult Result) noexcept
{
	switch (Result)
	{
	case VK_SUCCESS:
		return "VK_SUCCESS";
	case VK_NOT_READY:
		return "VK_NOT_READY";
	case VK_TIMEOUT:
		return "VK_TIMEOUT";
	case VK_EVENT_SET:
		return "VK_EVENT_SET";
	case VK_EVENT_RESET:
		return "VK_EVENT_RESET";
	case VK_INCOMPLETE:
		return "VK_INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "VK_ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED:
		return "VK_ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST:
		return "VK_ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "VK_ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "VK_ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "VK_ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "VK_ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "VK_ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "VK_ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_FRAGMENTED_POOL:
		return "VK_ERROR_FRAGMENTED_POO";
	case VK_ERROR_UNKNOWN:
		return "VK_ERROR_UNKNOWN";
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		return "VK_ERROR_OUT_OF_POOL_MEMORY";
	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
		return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
	case VK_ERROR_FRAGMENTATION:
		return "VK_ERROR_FRAGMENTATION";
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
		return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
	case VK_PIPELINE_COMPILE_REQUIRED:
		return "VK_PIPELINE_COMPILE_REQUIRED";
	case VK_ERROR_SURFACE_LOST_KHR:
		return "VK_ERROR_SURFACE_LOST_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_SUBOPTIMAL_KHR:
		return "VK_SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "VK_ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "VK_ERROR_VALIDATION_FAILED_EXT";
	case VK_ERROR_INVALID_SHADER_NV:
		return "VK_ERROR_INVALID_SHADER_NV";
	case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
		return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
	case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
		return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
		return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
	case VK_ERROR_NOT_PERMITTED_KHR:
		return "VK_ERROR_NOT_PERMITTED_KHR";
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
		return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
	case VK_THREAD_IDLE_KHR:
		return "VK_THREAD_IDLE_KHR";
	case VK_THREAD_DONE_KHR:
		return "VK_THREAD_DONE_KHR";
	case VK_OPERATION_DEFERRED_KHR:
		return "VK_OPERATION_DEFERRED_KHR";
	case VK_OPERATION_NOT_DEFERRED_KHR:
		return "VK_OPERATION_NOT_DEFERRED_KHR";
	case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
		return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
	case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
		return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
	case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
		return "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
	default:
		break;
	}

	return "Undefined Error!";
}
