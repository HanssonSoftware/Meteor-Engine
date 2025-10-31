/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Array.h>
#include <Types/String.h>
#include <Module/Module.h>

#include <Platform/File.h>


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

protected:
	bool ReadArguments();

	const ERequestedCommandToDo GetBuildCommand() const { return command; }

protected:


	Array<Module> loadedModules;
};

