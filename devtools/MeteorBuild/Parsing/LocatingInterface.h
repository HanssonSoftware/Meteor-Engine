/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Parsing/Module.h>
#include <vector>

struct String;

struct Locator
{
	static bool FindAllReferences(const String& sourceDirectory);

	static void ListDirectory(const String& directory, std::vector<String>& returned);

	static void SearchExtensionSpecified(const String& directory, const String& extension,std::vector<String>& returned);

	inline static std::vector<Module*> modules;

	inline static std::vector<String> foundPaths;
};

