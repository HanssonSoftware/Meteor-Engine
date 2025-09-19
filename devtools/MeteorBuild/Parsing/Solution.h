/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

#include <rpc.h>

struct ProjectDefine
{
	GUID projectType;

	String projectName;

	String pathToProject;

	GUID uniqueGeneratedID;

	String dependencyProject;
};

class Solution
{
public:

	Array<ProjectDefine*> projects;

	bool Finalize(String* bufferToWrite);
};

