/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Array.h>
#include <Types/String.h>
#include <Module/Module.h>

#include <Platform/File.h>

class ProjectScript;

class BuildSystem
{
protected:
	enum ERequestedCommandToDo
	{
		Build,
		Rebuild

	} command;
public:
	bool InitFramework();

	void OrderModules();

	bool BuildProjectFiles();

	const Array<Module>* GetModules() const { return &loadedModules; };

	ProjectScript& GetProjectScript() const { return *ps; }
protected:
	bool ReadArguments();

	const ERequestedCommandToDo GetBuildCommand() const { return command; }

	ProjectScript* ps;

	Array<Module> loadedModules;
};

