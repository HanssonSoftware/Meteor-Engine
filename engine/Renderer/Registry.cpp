/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Registry.h"
#include "OutputContext.h"
#include <Logging/LogMacros.h>
#include <Platform/PlatformLayout.h>

#include <Core/Application.h>
#include <Platform/PerformanceTimer.h>
#include <Platform/File.h>
#include <Layers/SystemLayer.h>

#include <Platform/FileManager.h>

#define STR(x) #x

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

	return /*VK_TRUE*/ 1U;
}

static String privRHIName;
String VulkanRegistry::GetName() const
{
	static constexpr const wchar_t* vkRHIDisplayName = L"Vulkan";

	if (privRHIName.IsEmpty()) privRHIName = vkRHIDisplayName;

	return privRHIName;
}

bool VulkanRegistry::Initialize()
{
	context = new VulkanOutputContext();

	bool bError = false;
	bError = !CreateInstance();

	CreateDebugMessenger();

	bError = !CreateSurfaceWin32();

	bError = !CreateQueueSlots();

	bError = !CreateDevice();

	bError = !CreateSwapchain();

	bError = !CreateImageViews();

	bError = !CreateRenderpass();

	bError = !CreateFramebuffers();

	VulkanOutputContext* ctx = (VulkanOutputContext*)context;
	ctx->CreateCommandBuffers();

	//initVK();

	if (bError) CleanUp();
	return true;
}

void VulkanRegistry::CleanUp() const
{
	context->CleanUp();
	
	for (uint32 i = 0; i < swapChainImagesCount; i++)
	{
		vkDestroyFramebuffer(selectedDevice, swapChainFramebuffers[i], nullptr);
		swapChainFramebuffers[i] = nullptr;
	}

	delete[] swapChainFramebuffers;

	vkDestroyRenderPass(selectedDevice, renderPass, nullptr);

	vkDestroyPipeline(selectedDevice, pipeline, 0);

	vkDestroyShaderModule(selectedDevice, shaderModuleVert, 0);

	vkDestroyShaderModule(selectedDevice, shaderModuleFrag, 0);

	for (uint32 j = 0; j < swapChainImagesCount; j++)
	{
		vkDestroyImageView(selectedDevice, swapChainImageViews[j], nullptr);
		swapChainImageViews[j] = nullptr;
	}

	delete[] swapChainImageViews;

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

bool VulkanRegistry::CreateSurfaceWin32()
{
	VkWin32SurfaceCreateInfoKHR win32SurfaceInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
	if (WindowsWindowManager* windowManager = (WindowsWindowManager*)Application::Get()->GetWindowManager())
	{
		win32SurfaceInfo.hinstance = windowManager->GetInstance();
		win32SurfaceInfo.hwnd = (HWND)windowManager->GetFirstWindow()->GetWindowHandle();
	}
	else
	{
		MR_LOG(LogVulkan, Warn, "Unable to get window manager! Using fallback method!");

		win32SurfaceInfo.hinstance = GetModuleHandle(0);
		win32SurfaceInfo.hwnd = GetFocus();
	}

	const VkResult result = vkCreateWin32SurfaceKHR(instance, &win32SurfaceInfo, 0, &surface);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateWin32SurfaceKHR returned: %s", STR(result));
		return false;
	}

	return true;
}

bool VulkanRegistry::CreateQueueSlots()
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
			continue;

		std::vector<VkQueueFamilyProperties> physicalDeviceQueueFamilies(physicalDeviceQueueFamiliesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueFamiliesCount, physicalDeviceQueueFamilies.data());

		for (uint32 i = 0; i < physicalDeviceQueueFamiliesCount; i++)
		{
			VkQueueFamilyProperties& queueFamilyProps = physicalDeviceQueueFamilies[i];

			if (queueFamilyProps.queueCount == 0)
				continue;

			int availableQueues = queueFamilyProps.queueCount;

			if (queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				VkBool32 bIsGood = 0;
				vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &bIsGood);
				if (bIsGood)
				{
					QueueIndex = i;
					selectedVirtualDevice = physicalDevice;
					return true;
				}
			}
		}
	}

	return false;
}

bool VulkanRegistry::CreateDevice()
{
	VkDeviceCreateInfo deviceInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };

	static constexpr const char* requiredDeviceExtensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	static constexpr const uint32 requiredDeviceExtensionsCount = sizeof(requiredDeviceExtensions) / sizeof(requiredDeviceExtensions[0]);

	deviceInfo.enabledExtensionCount = requiredDeviceExtensionsCount;
	deviceInfo.ppEnabledExtensionNames = requiredDeviceExtensions;

	VkDeviceQueueCreateInfo queueOne = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	constexpr float priority = 1.f;
		
	queueOne.pQueuePriorities = &priority;
	queueOne.queueCount = 1;
	queueOne.queueFamilyIndex = QueueIndex;
		
	deviceInfo.pQueueCreateInfos = &queueOne;
	deviceInfo.queueCreateInfoCount = 1;

	
	const VkResult result = vkCreateDevice(selectedVirtualDevice, &deviceInfo, 0, &selectedDevice);
	if (result == VK_ERROR_EXTENSION_NOT_PRESENT)
	{
		uint32 extensionsCount = 0;
		vkEnumerateDeviceExtensionProperties(selectedVirtualDevice, nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionsCount);

		vkEnumerateDeviceExtensionProperties(selectedVirtualDevice, nullptr, &extensionsCount, extensions.data());
		
		const uint32 foundExtensions = (uint32)extensions.size();
		
		std::vector<const char*> unsupportedExtensions;
		for (uint32 i = 0; i < requiredDeviceExtensionsCount; i++)
		{
			bool bHasFound = false;
			const char* criticalExtensionIndex = requiredDeviceExtensions[i];

			for (uint32 j = 0; j < extensionsCount; j++)
			{
				VkExtensionProperties& extProp = extensions[j];
				if (strcmp(criticalExtensionIndex, extProp.extensionName) == 0)
				{
					bHasFound = true;
					break;
				}
			}

			if (!bHasFound) unsupportedExtensions.push_back(criticalExtensionIndex);
		}

		const uint32 extSize = (uint32)unsupportedExtensions.size();
		String super;
		for (uint32 i = 0; i < extSize; i++)
		{
			super += unsupportedExtensions[i];
			if (extSize > 1)
				super += ", ";
		}

		MR_LOG(LogVulkan, Fatal, "Vulkan extension(s) are not supported! %s", super.Chr());
		// return false;
	}
	else if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateDevice returned: %s", STR(result));
		return false;
	}

	vkGetDeviceQueue(selectedDevice, QueueIndex, 0, &graphicsQueue);

	return true;
}

bool VulkanRegistry::CreateSwapchain()
{
	surfaceCapabilities = {};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(selectedVirtualDevice, surface, &surfaceCapabilities);


	VkSwapchainCreateInfoKHR swapChainInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	swapChainInfo.surface = surface;
	swapChainInfo.minImageCount = surfaceCapabilities.minImageCount + 1;
	swapChainInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
	swapChainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;


	swapChainInfo.imageExtent = surfaceCapabilities.maxImageExtent;
	swapChainInfo.imageArrayLayers = 1;
	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapChainInfo.preTransform = surfaceCapabilities.currentTransform;
	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	swapChainInfo.clipped = 1;

	const VkResult result = vkCreateSwapchainKHR(selectedDevice, &swapChainInfo, 0, &swapChain);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateSwapchainKHR returned: %s", STR(result));
		return false;
	}

	
	vkGetSwapchainImagesKHR(selectedDevice, swapChain, &swapChainImagesCount, nullptr);

	swapChainImages = new VkImage[swapChainImagesCount]();
	swapChainImageViews = new VkImageView[swapChainImagesCount]();

	vkGetSwapchainImagesKHR(selectedDevice, swapChain, &swapChainImagesCount, swapChainImages);

	return true;
}

bool VulkanRegistry::CreateImageViews()
{
	for (uint32 i = 0; i < swapChainImagesCount; i++)
	{
		VkImageViewCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		vkCreateImageView(selectedDevice, &createInfo, 0, &swapChainImageViews[i]);
	}

	return true;
}

bool VulkanRegistry::CreateFramebuffers()
{
	swapChainFramebuffers = new VkFramebuffer[swapChainImagesCount]();

	for (uint32 i = 0; i < swapChainImagesCount; i++)
	{
		VkFramebufferCreateInfo framebufferCreateInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
		framebufferCreateInfo.renderPass = renderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &swapChainImageViews[i];
		framebufferCreateInfo.width = surfaceCapabilities.maxImageExtent.width;
		framebufferCreateInfo.height = surfaceCapabilities.maxImageExtent.height;
		framebufferCreateInfo.layers = 1;

		const VkResult result = vkCreateFramebuffer(selectedDevice, &framebufferCreateInfo, nullptr, &swapChainFramebuffers[i]);
		if (result != VK_SUCCESS)
		{
			MR_LOG(LogVulkan, Error, "vkCreateFramebuffer returned: %s", STR(result));
			return false;
		}
	}

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

	const VkResult result = vkCreateRenderPass(selectedDevice, &renderPassInfo, nullptr, &renderPass);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateRenderPass returned: %x", result);
		return false;
	}

	return true;
}

VkShaderModule VulkanRegistry::OpenShader(const String path) const
{
	FileStatus stat;
	IFile* shader = FileManager::CreateFileOperation(path, OPENMODE_READ, SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS, stat);
	if (shader)
	{
		shader->Read();

		VkShaderModuleCreateInfo smoduleInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
		smoduleInfo.codeSize = shader->GetSize();
		smoduleInfo.pCode = (uint32_t*)shader->GetBuffer();

		VkShaderModule module = nullptr;
		const VkResult result = vkCreateShaderModule(selectedDevice, &smoduleInfo, nullptr, &module);
		if (result != VK_SUCCESS)
		{
			MR_LOG(LogVulkan, Error, "Failed to create shader module! %d", result);
			return nullptr;
		}

		return module;
	}

	MR_LOG(LogVulkan, Error, "Failed to load shader file! %i", stat);
	return nullptr;
}

bool VulkanRegistry::initVK()
{
	{
		shaderModuleVert = OpenShader("E:\\meteor\\editor\\vert.spv");
		shaderModuleFrag = OpenShader("E:\\meteor\\editor\\frag.spv");
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

		VkGraphicsPipelineCreateInfo gfxPp = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

		static constexpr const VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
		dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
		dynamicState.pDynamicStates = dynamicStates;

		const VkResult result = /*vkCreateGraphicsPipelines(selectedDevice, nullptr, 1, 0,0,0)*/VK_SUCCESS;
		if (result != VK_SUCCESS)
		{
			MR_LOG(LogVulkan, Error, "vkCreatePipelineLayout returned: %s", STR(result));
			return false;
		}
	}

	return true; 
}

#ifdef MR_DEBUG
static constexpr const char* layers[]
{
	"VK_LAYER_KHRONOS_validation"
};
#endif // MR_DEBUG

bool VulkanRegistry::CreateInstance()
{
	VkInstanceCreateInfo instanceInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };

	static constexpr const char* extensions[]
	{
#ifdef MR_DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif // MR_DEBUG
		VK_KHR_SURFACE_EXTENSION_NAME,


#ifdef MR_PLATFORM_WINDOWS
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif // MR_PLATFORM_WINDOWS
	};

	VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	appInfo.apiVersion = VK_API_VERSION_1_3;

	instanceInfo.enabledExtensionCount = (uint32)sizeof(extensions) / sizeof(extensions[0]);
	instanceInfo.ppEnabledExtensionNames = extensions;

#ifdef MR_DEBUG
	instanceInfo.ppEnabledLayerNames = layers;
	instanceInfo.enabledLayerCount = (uint32)sizeof(layers) / sizeof(layers[0]);
#else
	instanceInfo.ppEnabledLayerNames = nullptr;
	instanceInfo.enabledLayerCount = 0;
#endif // MR_DEBUG

	instanceInfo.pApplicationInfo = &appInfo;

	const VkResult result = vkCreateInstance(&instanceInfo, 0, &instance);
	if (result != VK_SUCCESS)
	{
		MR_LOG(LogVulkan, Error, "vkCreateInstance returned: %s", STR(result));
		return false;
	};

	return true;
}

bool VulkanRegistry::GetInstanceLayerCompatibility()
{
#ifdef MR_DEBUG
	if constexpr (bIsUsingValidation)
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

		return bFound ? true : false;
	}

#endif // MR_DEBUG

	return false;
}

bool VulkanRegistry::CreateDebugMessenger()
{
	constexpr VkDebugUtilsMessengerCreateInfoEXT dbmonInfo = { 
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		nullptr,
		0,
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,

		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		debugCallback,
		nullptr
	};

	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsEXT =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (vkCreateDebugUtilsEXT)
	{
		const VkResult result = vkCreateDebugUtilsEXT(instance, &dbmonInfo, 0, &dbMon);
		if (result != VK_SUCCESS)
		{
			MR_LOG(LogVulkan, Error, "vkCreateDebugUtilsEXT (vkCreateDebugUtilsMessengerEXT) returned: %s", STR(result));
			return false;
		}
	}
	else
	{
		MR_LOG(LogVulkan, Warn, "Debug utils requested, but not available!");
	}

	return true;
}

