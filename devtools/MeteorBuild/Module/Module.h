/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

#include <rpc.h>

class Module
{
public:
	Module() = default;

	bool Parse(String* modulePath);

	String moduleName;

	String dependsOn;

	Array<String> files;

	Array<String> includePaths;

	Array<String> requires;

	GUID identification;
};

