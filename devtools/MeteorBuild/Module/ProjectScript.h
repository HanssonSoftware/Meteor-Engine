/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

#include <rpc.h>

struct ProjectDependencies
{
	String projectName;

	String projectPath;

	GUID projectGUID;

	bool bIsFolder = false;
};

class ProjectScript
{
public:
	String projectName;

	Array<ProjectDependencies> projects;

	bool Finalize(String& output);

	void Parse(String* input);
};

