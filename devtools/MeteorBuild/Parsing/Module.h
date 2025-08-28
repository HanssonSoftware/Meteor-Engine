/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Platform/File.h>
#include <Types/Array.h>

class Module
{
	friend class ScriptParser;
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

		Array<String> includePaths;

		Array<String> dependencyModules;

		Array<String> linkedLibraries;
	};

	String fullPath;

	Array<String> includedSources;

	IFile* moduleDescriptor = nullptr;
};

