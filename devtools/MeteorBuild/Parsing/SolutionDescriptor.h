/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>

#include <rpc.h>

class IFile;

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
	bool Finalize(String* bufferToWrite);
	
	bool OpenProject(const String& projectPath);

	static constexpr const char* GetGUIDToString(EProjectType type);
	
	Array<ProjectDefine*> projects;

	IFile* file = nullptr;
};

constexpr const char* SolutionDescriptor::GetGUIDToString(EProjectType type)
{
	switch (type)
	{
		case EProjectType::CPP:
			return "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}";
		case EProjectType::Folder:
			return "{2150E333-8FDC-42A3-9474-1A3956D46DE8}";
	}
	
	return "";
}

