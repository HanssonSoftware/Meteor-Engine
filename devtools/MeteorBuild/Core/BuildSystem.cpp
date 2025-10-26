/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "BuildSystem.h"

#include <Commandlet.h>
#include <Parsing/Finder.h>

LOG_ADDCATEGORY(BuildSystemFramework);

void BuildSystem::ReadAndParseArguments()
{
	if (Commandlet::Parse("-build", nullptr))
	{
		command = Build;
	}
	else if (Commandlet::Parse("-rebuild", nullptr))
	{
		command = Rebuild;
	}
	else
	{
		MR_LOG(LogBuildSystemFramework, Fatal, "Main parameter is missing! build/rebuild");
	}
}

void BuildSystem::SearchScriptFiles()
{
	String sourceDirectoryFromLaunchParameter;
	if (Commandlet::Parse("-source", &sourceDirectoryFromLaunchParameter))
	{
		Finder::LocateSources(sourceDirectoryFromLaunchParameter, pathToDiscoveredItems);

		for (auto& a : pathToDiscoveredItems)
		{
			MR_LOG(LogBuildSystemFramework, Log, "Found: %s", *a);
		}
	}
}
