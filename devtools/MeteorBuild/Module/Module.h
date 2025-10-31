/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

class Module
{
public:
	Module() = default;

	void Parse(String* modulePath);

	String moduleName;

	String dependsOn;

	Array<String> includePaths;
};

