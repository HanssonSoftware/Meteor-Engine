/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <vector>

class FMFile;

class DirectoryExplorer
{
	friend class FMFile;
public:
	DirectoryExplorer();

	~DirectoryExplorer();

	inline void goBack(String& input);

	void startExpedition(String directory);

	std::vector<FMFile*> processModules();

	bool processToModuleDescriptor(const wchar_t* buffer);

	std::vector<String> modules;
};

