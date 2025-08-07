/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Array.h>
#include <Types/String.h>

class ModuleManager
{
public:
	static ModuleManager& Get();

	bool LoadModule(const String& moduleName);


protected:
	virtual ~ModuleManager() noexcept;

	Array<String> modules;

	// static ModuleManager object; // This must be an extensible class!
};

