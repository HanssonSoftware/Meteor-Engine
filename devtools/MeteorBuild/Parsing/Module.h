/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Platform/File.h>
#include <vector>

class Module
{
public:
	Module() = default;
	Module(const Module&) = delete;

	virtual ~Module() noexcept = default;

	static Module* CreateModule(const String fullPathToModule);

	void OpenPath(const String& fullPathToModule);

	IFile* GetModuleBuffer() const { return moduleDescriptor; }

private:
	String name;

	String path;

	IFile* moduleDescriptor = nullptr;
};

