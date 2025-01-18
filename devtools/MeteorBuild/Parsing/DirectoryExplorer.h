/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <vector>

class DirectoryExplorer
{
public:
	DirectoryExplorer();

	~DirectoryExplorer();

	inline void goBack(String& input);

	void startExpedition(String directory);

	void processModules();

	std::vector<String> modules;
};

