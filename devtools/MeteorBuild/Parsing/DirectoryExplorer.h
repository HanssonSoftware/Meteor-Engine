/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <vector>

class IFile;

class DirectoryExplorer
{
	friend class IFile;
public:
	DirectoryExplorer();

	~DirectoryExplorer();

	inline void GoBack(String& input);

	void StartExpedition(String directory);

	std::vector<IFile*> processModules();

	bool ProcessToModuleDescriptor(const char* buffer);

	std::vector<String> modules;
};

