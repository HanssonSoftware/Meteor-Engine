/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Registry.h"
#include "OutputContext.h"
#include "SwapChain.h"
#include "Device.h"
#include <Logging/LogMacros.h>
#include <Platform/PlatformLayout.h>

#include <Core/Application.h>
#include <Platform/PerformanceTimer.h>
#include <Platform/File.h>
#include <Layers/SystemLayer.h>

#include <Platform/FileManager.h>

LOG_ADDCATEGORY(Vulkan);

#ifdef MR_DEBUG
static constexpr const bool bIsUsingValidation = true;
#else
static constexpr const bool bIsUsingValidation = false;
#endif

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

	return VK_TRUE;
}

String VulkanRegistry::GetName()
{
	return "Vulkan";
}

bool VulkanRegistry::Initialize()
{
	registry = new VulkanRegistry;

	if (!registry->CreateInstance())
		return false;

	registry->CreateDebugMessenger();

	registry->swapChain = new VulkanSwapChain;

	if (!registry->swapChain->CreateSurface())
		return false;

	if (!registry->FindQueues())
		return false;

	if (!registry->CreateDevice())
		return false;

	if (!registry->swapChain->CreateSwapchain())
		return false;

	if (!registry->CreateRenderpass())
		return false;

	if (!registry->swapChain->CreateFramebuffers())
		return false;

	registry->outputContext = new VulkanOutputContext;

	if (!registry->outputContext->CreateCommandBuffers())
		return false;

	return true;
}

#ifdef MR_DEBUG
static constexpr const char* layers[]
{
	"VK_LAYER_KHRONOS_validation"
};
#endif // MR_DEBUG

static constexpr const char* instanceExtensions[]
{
#ifdef MR_DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif // MR_DEBUG

		VK_KHR_SURFACE_EXTENSION_NAME,

#ifdef MR_PLATFORM_WINDOWS
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif // MR_PLATFORM_WINDOWS
};

bool VulkanRegistry::CreateInstance()
{
	uint32 version = 0;
	vkEnumerateInstanceVersion(&version);
	if (version < VK_API_VERSION_1_3)
	{
		MR_LOG(LogVulkan, Fatal, "Vulkan is not compatible with your system! Consider upgrading.");
	}

	constexpr VkApplicationInfo appInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,		// sType
		nullptr,								// pNext
		"",										// pApplicationName
		0,										// applicationVersion
		"",										// pEngineName
		0,										// engineVersion
		VK_API_VERSION_1_3						// apiVersion
	};

	VkInstanceCreateInfo instanceInfo = { 
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,										// sType
		nullptr,																	// pNext
		0,																			// flags
		&appInfo,																	// pApplicationInfo
#ifdef MR_DEBUG
		(uint32)sizeof(layers) / sizeof(layers[0]),									// enabledLayerCount
		layers,																		// ppEnabledLayerNames
#else
		0,																			// enabledLayerCount
		nullptr,																	// ppEnabledLayerNames
#endif																									
		(uint32)sizeof(instanceExtensions) / sizeof(instanceExtensions[0]),			// enabledExtensionCount
		instanceExtensions															// ppEnabledExtensionNames
	};

	const VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateInstance returned: %s", VkResultToText(result));
		return false;
	};

	return true;
}

void VulkanRegistry::CreateDebugMessenger()
{
	constexpr VkDebugUtilsMessengerCreateInfoEXT dbmonInfo = {
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,		// sType
		nullptr,														// pNext
		0,																// flags
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |				// messageSeverity
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |				 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,					 
																		
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |					// messageType
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |				
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		debugCallback,													// pfnUserCallback
		nullptr															// pUserData
	};

	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsEXT =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (vkCreateDebugUtilsEXT)
	{
		const VkResult result = vkCreateDebugUtilsEXT(instance, &dbmonInfo, 0, &debugger);
		if (result != VK_SUCCESS)
		{
			MR_LOG(LogVulkan, Error, "vkCreateDebugUtilsEXT (vkCreateDebugUtilsMessengerEXT) returned: %s", VkResultToText(result));
		}
	}
	else
	{
		MR_LOG(LogVulkan, Warn, "Debug utils requested, but not available!");
	}
}

void VulkanRegistry::DestroyDebugMessenger() const
{
	constexpr VkDebugUtilsMessengerCreateInfoEXT dbmonInfo = { 
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,			// sType
		nullptr,															// pNext
		0,																	// flags
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |						// messageSeverity
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,																	
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |						// messageType
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,																	
		debugCallback,														// pfnUserCallback
		nullptr																// pUserData
	};

	PFN_vkDestroyDebugUtilsMessengerEXT vkDestDebugUtilsEXT =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (vkDestDebugUtilsEXT)
	{
		vkDestDebugUtilsEXT(instance, debugger, nullptr);
	}
}

bool VulkanRegistry::FindQueues()
{
	uint32 physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(registry->instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(registry->instance, &physicalDeviceCount, physicalDevices.data());

	for (VkPhysicalDevice& physicalDevice : physicalDevices)
	{
		uint32 queueFamilyPropertiesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());
		
		for (uint32 i = 0; i < queueFamilyPropertiesCount; i++)
		{
			VkQueueFamilyProperties& familyProperties = queueFamilyProperties[i];
			
			if (familyProperties.queueCount == 0)
				continue;

			if (familyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				VkBool32 bIsSupported = 0;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, swapChain->surface, &bIsSupported);
				if (bIsSupported)
				{
					device = new VulkanDevice;
					
					device->graphicsQueueIndex = i;
					device->physicalDevice = physicalDevice;

					return true;
				}
			}
		}
	}

	return false;
}

bool VulkanRegistry::CreateDevice()
{
	static constexpr const char* requiredDeviceExtensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};


	constexpr float priority = 1.f;
	const VkDeviceQueueCreateInfo queueOne = { 
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,		// sType
		nullptr,										// pNext
		0,												// flags
		(uint32)device->graphicsQueueIndex,				// queueFamilyIndex
		1,												// queueCount
		&priority										// pQueuePriorities
	};


	const VkDeviceCreateInfo deviceInfo = { 
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,											// sType
		nullptr,																		// pNext
		0,																				// flags
		1,																				// queueCreateInfoCount
		&queueOne,																		// pQueueCreateInfos
		0, /* Deprecated */																// enabledLayerCount
		nullptr, /* Deprecated */														// ppEnabledLayerNames
		sizeof(requiredDeviceExtensions) / sizeof(requiredDeviceExtensions[0]),			// enabledExtensionCount
		requiredDeviceExtensions,														// ppEnabledExtensionNames
		nullptr																			// pEnabledFeatures
	};



	const VkResult result = vkCreateDevice(device->physicalDevice, &deviceInfo, nullptr, &device->device);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateDevice returned: %s", VkResultToText(result));
		return false;
	}

	vkGetPhysicalDeviceFeatures(device->physicalDevice, &device->deviceFeatures);
	vkGetDeviceQueue(device->device, device->graphicsQueueIndex, 0, &device->graphicsQueue);

	return true;
}

bool VulkanRegistry::CreateRenderpass()
{
	VkAttachmentDescription attachmentInfo = {};
	attachmentInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentInfo.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachmentInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	attachmentInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentInfo.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentInfo.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency subpassD = {};
	subpassD.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassD.dstSubpass = 0;
	subpassD.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassD.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassD.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassD.srcAccessMask = 0;

	VkRenderPassCreateInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
	renderPassInfo.pAttachments = &attachmentInfo;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &subpassD;

	const VkResult result = vkCreateRenderPass(registry->device->device, &renderPassInfo, nullptr, &renderPass);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateRenderPass returned: %s", VkResultToText(result));
		return false;
	}

	return true;
}

void VulkanRegistry::Shutdown()
{
	registry->outputContext->CleanUp();

	vkDestroyRenderPass(registry->device->device, registry->renderPass, nullptr);

	registry->swapChain->Shutdown();

	vkDestroyDevice(registry->device->device, nullptr);

	registry->DestroyDebugMessenger();

	vkDestroyInstance(registry->instance, nullptr);

	delete registry->device, registry;
}

void VulkanRegistry::Render()
{
	outputContext->Draw();
}

VulkanRegistry* VulkanRegistry::GetRegistry()
{
	return registry;
}

//bool VulkanRegistry::CreateQueueSlots()
//{
//	uint32 physicalDeviceCount = 0;
//	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, 0);
//	if (physicalDeviceCount == 0)
//	{
//		MR_LOG(LogVulkan, Error, "Failed to find any physical device!");
//		return false;
//	}
//
//	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
//	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
//
//	for (VkPhysicalDevice& physicalDevice : physicalDevices)
//	{
//		uint32 physicalDeviceQueueFamiliesCount = 0;
//		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamiliesCount, 0);
//		if (physicalDeviceQueueFamiliesCount == 0)
//			continue;
//
//		std::vector<VkQueueFamilyProperties> physicalDeviceQueueFamilies(physicalDeviceQueueFamiliesCount);
//		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamiliesCount, physicalDeviceQueueFamilies.data());
//
//		for (uint32 i = 0; i < physicalDeviceQueueFamiliesCount; i++)
//		{
//			VkQueueFamilyProperties& queueFamilyProps = physicalDeviceQueueFamilies[i];
//
//			if (queueFamilyProps.queueCount == 0)
//				continue;
//
//			int availableQueues = queueFamilyProps.queueCount;
//
//			if (queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//			{
//				VkBool32 bIsGood = 0;
//				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &bIsGood);
//				if (bIsGood)
//				{
//					QueueIndex = i;
//					selectedVirtualDevice = physicalDevice;
//					return true;
//				}
//			}
//		}
//	}
//
//	return false;
//}
//

//
//bool VulkanRegistry::CreateSwapchain()
//{
//	surfaceCapabilities = {};
//	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(selectedVirtualDevice, surface, &surfaceCapabilities);
//
//
//	VkSwapchainCreateInfoKHR swapChainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
//	swapChainInfo.surface = surface;
//	swapChainInfo.minImageCount = surfaceCapabilities.minImageCount + 1;
//	swapChainInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
//	swapChainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
//
//
//	swapChainInfo.imageExtent = surfaceCapabilities.maxImageExtent;
//	swapChainInfo.imageArrayLayers = 1;
//	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//	swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//	swapChainInfo.preTransform = surfaceCapabilities.currentTransform;
//	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//	swapChainInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
//	swapChainInfo.clipped = 1;
//
//	const VkResult result = vkCreateSwapchainKHR(selectedDevice, &swapChainInfo, 0, &swapChain);
//	if (result != VK_SUCCESS)
//	{
//		MR_LOG(LogVulkan, Error, "vkCreateSwapchainKHR returned: %s", STR(result));
//		return false;
//	}
//
//	
//	vkGetSwapchainImagesKHR(selectedDevice, swapChain, &swapChainImagesCount, nullptr);
//
//	swapChainImages = new VkImage[swapChainImagesCount]();
//	swapChainImageViews = new VkImageView[swapChainImagesCount]();
//
//	vkGetSwapchainImagesKHR(selectedDevice, swapChain, &swapChainImagesCount, swapChainImages);
//
//	return true;
//}
//
//bool VulkanRegistry::CreateImageViews()
//{
//	for (uint32 i = 0; i < swapChainImagesCount; i++)
//	{
//		VkImageViewCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
//		createInfo.image = swapChainImages[i];
//		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
//		createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
//		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
//		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
//		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
//		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
//		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//		createInfo.subresourceRange.baseMipLevel = 0;
//		createInfo.subresourceRange.levelCount = 1;
//		createInfo.subresourceRange.baseArrayLayer = 0;
//		createInfo.subresourceRange.layerCount = 1;
//
//		vkCreateImageView(selectedDevice, &createInfo, 0, &swapChainImageViews[i]);
//	}
//
//	return true;
//}
//
//bool VulkanRegistry::CreateFramebuffers()
//{
//	swapChainFramebuffers = new VkFramebuffer[swapChainImagesCount]();
//
//	for (uint32 i = 0; i < swapChainImagesCount; i++)
//	{
//		VkFramebufferCreateInfo framebufferCreateInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
//		framebufferCreateInfo.renderPass = renderPass;
//		framebufferCreateInfo.attachmentCount = 1;
//		framebufferCreateInfo.pAttachments = &swapChainImageViews[i];
//		framebufferCreateInfo.width = surfaceCapabilities.maxImageExtent.width;
//		framebufferCreateInfo.height = surfaceCapabilities.maxImageExtent.height;
//		framebufferCreateInfo.layers = 1;
//
//		const VkResult result = vkCreateFramebuffer(selectedDevice, &framebufferCreateInfo, nullptr, &swapChainFramebuffers[i]);
//		if (result != VK_SUCCESS)
//		{
//			MR_LOG(LogVulkan, Error, "vkCreateFramebuffer returned: %s", STR(result));
//			return false;
//		}
//	}
//
//	return true;
//}


//
//VkShaderModule VulkanRegistry::OpenShader(const String path) const
//{
//	FileStatus stat;
//	IFile* shader = FileManager::CreateFileOperation(path, OPENMODE_READ, SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS, stat);
//	if (shader)
//	{
//		shader->Read();
//
//		VkShaderModuleCreateInfo smoduleInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
//		smoduleInfo.codeSize = shader->GetSize();
//		smoduleInfo.pCode = (uint32_t*)shader->GetBuffer();
//
//		VkShaderModule module = nullptr;
//		const VkResult result = vkCreateShaderModule(selectedDevice, &smoduleInfo, nullptr, &module);
//		if (result != VK_SUCCESS)
//		{
//			MR_LOG(LogVulkan, Error, "Failed to create shader module! %d", result);
//			return nullptr;
//		}
//
//		return module;
//	}
//
//	MR_LOG(LogVulkan, Error, "Failed to load shader file! %i", stat);
//	return nullptr;
//}
//
//bool VulkanRegistry::initVK()
//{
//	{
//		shaderModuleVert = OpenShader("E:\\meteor\\editor\\vert.spv");
//		shaderModuleFrag = OpenShader("E:\\meteor\\editor\\frag.spv");
//	}
//
//	{
//		VkPipelineShaderStageCreateInfo shaderInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
//		shaderInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
//		shaderInfo.module = shaderModuleVert;
//		shaderInfo.pName = "main";		
//		
//		VkPipelineShaderStageCreateInfo shaderInfoA = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
//		shaderInfoA.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//		shaderInfoA.module = shaderModuleFrag;
//		shaderInfoA.pName = "main";
//
//		VkPipelineShaderStageCreateInfo shaderStages[] = { shaderInfo, shaderInfoA };
//
//
//		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
//		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//		vertexInputInfo.vertexBindingDescriptionCount = 0;
//		vertexInputInfo.vertexAttributeDescriptionCount = 0;
//	
//		VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
//		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//		inputAssembly.primitiveRestartEnable = VK_FALSE;
//
//		VkViewport viewport{};
//		viewport.x = 0.0f;
//		viewport.y = 0.0f;
//		viewport.width = (float)surfaceCapabilities.maxImageExtent.width;
//		viewport.height = (float)surfaceCapabilities.maxImageExtent.height;
//		viewport.minDepth = 0.0f;
//		viewport.maxDepth = 1.0f;
//
//		VkRect2D scissor{};
//		scissor.offset = { 0, 0 };
//		scissor.extent = surfaceCapabilities.maxImageExtent;
//
//		VkPipelineViewportStateCreateInfo viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
//		viewportState.viewportCount = 1;
//		viewportState.pViewports = &viewport;
//		viewportState.scissorCount = 1;
//		viewportState.pScissors = &scissor;
//
//		VkPipelineRasterizationStateCreateInfo rasterizer{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
//		rasterizer.depthClampEnable = VK_FALSE;
//		rasterizer.lineWidth = 1.0f;
//		rasterizer.rasterizerDiscardEnable = VK_FALSE;
//		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
//		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
//		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
//		rasterizer.depthBiasEnable = VK_FALSE;
//		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
//		rasterizer.depthBiasClamp = 0.0f; // Optional
//		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
//
//		VkPipelineMultisampleStateCreateInfo multisampling{};
//		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//		multisampling.sampleShadingEnable = VK_FALSE;
//		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//		multisampling.minSampleShading = 1.0f; // Optional
//		multisampling.pSampleMask = nullptr; // Optional
//		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
//		multisampling.alphaToOneEnable = VK_FALSE; // Optional
//
//		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
//		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//		colorBlendAttachment.blendEnable = VK_FALSE;
//		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
//		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
//		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
//		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
//		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
//		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
//
//		VkPipelineColorBlendStateCreateInfo colorBlending{};
//		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//		colorBlending.logicOpEnable = VK_FALSE;
//		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
//		colorBlending.attachmentCount = 1;
//		colorBlending.pAttachments = &colorBlendAttachment;
//		colorBlending.blendConstants[0] = 0.0f; // Optional
//		colorBlending.blendConstants[1] = 0.0f; // Optional
//		colorBlending.blendConstants[2] = 0.0f; // Optional
//		colorBlending.blendConstants[3] = 0.0f; // Optional
//
//		VkGraphicsPipelineCreateInfo gfxPp = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
//
//		static constexpr const VkDynamicState dynamicStates[] = {
//			VK_DYNAMIC_STATE_VIEWPORT,
//			VK_DYNAMIC_STATE_SCISSOR
//		};
//
//		VkPipelineDynamicStateCreateInfo dynamicState = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
//		dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
//		dynamicState.pDynamicStates = dynamicStates;
//
//		const VkResult result = /*vkCreateGraphicsPipelines(selectedDevice, nullptr, 1, 0,0,0)*/VK_SUCCESS;
//		if (result != VK_SUCCESS)
//		{
//			MR_LOG(LogVulkan, Error, "vkCreatePipelineLayout returned: %s", STR(result));
//			return false;
//		}
//	}
//
//	return true; 
//}
//


//bool VulkanRegistry::GetInstanceLayerCompatibility()
//{
//#ifdef MR_DEBUG
//	if constexpr (bIsUsingValidation)
//	{
//		uint32 layerCount = 0;
//		vkEnumerateInstanceLayerProperties(&layerCount, 0);
//
//		std::vector<VkLayerProperties> validationLayers(layerCount);
//		vkEnumerateInstanceLayerProperties(&layerCount, validationLayers.data());
//
//		bool bFound = false;
//		for (const char* layer : layers)
//		{
//			for (const auto& layerProp : validationLayers)
//			{
//				if (strcmp(layer, layerProp.layerName) == 0)
//				{
//					bFound = true;
//					break;
//				}
//			}
//		}
//
//		return bFound ? true : false;
//	}
//
//#endif // MR_DEBUG
//
//	return false;
//}
//

constexpr const char* VkResultToText(const VkResult& result)
{
	switch (result)
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

	return "";
}
