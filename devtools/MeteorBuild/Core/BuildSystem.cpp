/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "BuildSystem.h"

#include <Commandlet.h>
#include <FileManager.h>

#include <Platform/Platform.h>
#include "Utils.h"

LOG_ADDCATEGORY(BuildSystemFramework);

bool BuildSystem::InitFramework()
{
	if (ReadArguments())
	{
		String sourceDirectoryFromLaunchParameter;
		if (Commandlet::Parse("-source", &sourceDirectoryFromLaunchParameter))
		{
			Array<String> filesFoundInSources;
			Array<String> scriptsFound;

			Utils::ListDirectory(&sourceDirectoryFromLaunchParameter, filesFoundInSources);
			for (auto& pathToDiscoveredItemsIndexed : filesFoundInSources)
			{
				if (FileManager::IsEndingWith(pathToDiscoveredItemsIndexed, "mrbuild"))
				{
					scriptsFound.Add(pathToDiscoveredItemsIndexed);
					MR_LOG(LogBuildSystemFramework, Verbose, "Found: %s", *pathToDiscoveredItemsIndexed);
				}
			}

			
			for (auto& temp : scriptsFound)
			{
				Module mdl;

				mdl.Parse(&temp);

				loadedModules.Add(mdl);
			}
		}




		return true;
	}


	return false;
}

bool BuildSystem::ReadArguments()
{
	if (Commandlet::Parse("-build", nullptr))
	{
		command = Build;
		return true;
	}
	else if (Commandlet::Parse("-rebuild", nullptr))
	{
		command = Rebuild;
		return true;
	}
	else
	{
		MR_LOG(LogBuildSystemFramework, Fatal, "Main parameter is missing! build/rebuild");
	}

	return false;
}

