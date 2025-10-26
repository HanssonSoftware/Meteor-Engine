/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

#include <rpc.h>

class IFile;
class Solution;

enum class EProjectType
{
	CPP,
	Folder
};

struct ProjectDefine
{
	EProjectType projectType;

	String projectName;

	String pathToProject;

	GUID uniqueGeneratedID;

	String dependencyProject;
};

class SolutionDescriptor
{
public:
	static bool Finalize(Solution* slnToOutput, String* bufferToWrite);
	
	static IFile* OpenProject(String* projectPath);

	bool ParseDescriptor();
};
