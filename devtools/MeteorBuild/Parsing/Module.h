/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Platform/File.h>
#include <vector>

class IModule
{
public:
	IModule() = default;
	IModule(const IModule&) = delete;

	virtual ~IModule() noexcept = default;

	static IModule* CreateModule(const String fullPathToModule);

	static void Search(const String& dir, std::vector<String>& array);

	void OpenPath(const String& fullPathToModule);

	IFile* GetModuleBuffer() const { return moduleDescriptor; }
private:
	String name;

	String path;

	IFile* moduleDescriptor = nullptr;
};

