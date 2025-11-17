/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

class Module
{
public:
	Module() = default;

	bool Parse(String* modulePath);

	bool ConstructProjectFile(String* output);

	String moduleName;

	String modulePath;

	String dependsOn;

	Array<String> files;

	Array<String> includePaths;

	Array<String> requires;

	String identification;
};

