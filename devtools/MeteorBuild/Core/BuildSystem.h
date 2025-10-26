/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Array.h>
#include <Types/String.h>

class BuildSystem
{
protected:
	enum ERequestedCommandToDo
	{
		Build,
		Rebuild

	} command;
public:
	void ReadAndParseArguments();

	void SearchScriptFiles();

	void ParseDescriptorScript();

	const ERequestedCommandToDo GetBuildCommand() const { return command; }

protected:
	Array<String> pathToDiscoveredItems;

	Array<String> scripts;
};

