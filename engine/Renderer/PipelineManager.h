/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once

class VulkanPipelineManager
{
public:
	static VulkanPipelineManager& Get();


protected:


	static VulkanPipelineManager* object;
};

