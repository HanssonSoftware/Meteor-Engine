/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "PipelineManager.h"


VulkanPipelineManager& VulkanPipelineManager::Get()
{
	object = new VulkanPipelineManager;
	return *object;
}