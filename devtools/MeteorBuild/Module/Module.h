/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

class Module
{
public:
	Module() = default;

	virtual	~Module() noexcept = default;

	bool ConstructProjectFile(String* output);

	bool GetIsParsed() const { return bIsParsed; }

	void SetIsParsed(bool newVal) { bIsParsed = newVal; }

	String moduleName;

	String generatedProjectFile;

	String modulePath;

	String dependsOn;

	Array<String> files;

	Array<String> includePaths;

	Array<String> requires;

	String identification;

protected:
	bool bIsParsed = false;
};

