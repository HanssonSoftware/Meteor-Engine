/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <vector>
#include <Types/String.h>

class ModuleManager
{
public:
	static ModuleManager& Get();

	bool LoadModule(const String& moduleName);


protected:
	std::vector<String> modules;

	static ModuleManager object;
};

