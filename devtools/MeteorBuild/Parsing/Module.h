/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Platform/File.h>
#include <vector>

class Module
{
	friend struct ScriptParser;
public:
	virtual ~Module() noexcept = default;

	static Module* CreateModule(const String fullPathToModule);

	void OpenPath(const String& fullPathToModule);

	IFile* GetModuleBuffer() const { return moduleDescriptor; }

protected:
	Module() = default;

	Module(const Module&) = delete;

	struct
	{
		String moduleName;

		String solution;

		std::vector<String> includePaths;

		std::vector<String> dependencyModules;

		std::vector<String> linkedLibraries;
	};

	String fullPath;

	std::vector<String> includedSources;

	IFile* moduleDescriptor = nullptr;
};

