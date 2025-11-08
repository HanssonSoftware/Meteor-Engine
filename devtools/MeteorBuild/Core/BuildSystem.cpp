/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "BuildSystem.h"

#include <Commandlet.h>
#include <FileManager.h>

#include "Utils.h"

//#include <vector>

LOG_ADDCATEGORY(BuildSystemFramework);

bool BuildSystem::InitFramework()
{
	if (ReadArguments())
	{
		String sourceDirectoryFromLaunchParameter;
		if (Commandlet::Parse("-source", &sourceDirectoryFromLaunchParameter))
		{
			Array<FoundScriptData> filesFoundInSources;
			Array<FoundScriptData> scriptsFound;

			Utils::ListDirectory(&sourceDirectoryFromLaunchParameter, filesFoundInSources);
			for (auto& pathToDiscoveredItemsIndexed : filesFoundInSources)
			{
				//const String combined = String::Format("%s\\%s", *pathToDiscoveredItemsIndexed.path, *pathToDiscoveredItemsIndexed.name);
				if (FileManager::IsEndingWith(pathToDiscoveredItemsIndexed.full, "mrbuild"))
				{
					scriptsFound.Add(pathToDiscoveredItemsIndexed);
					MR_LOG(LogBuildSystemFramework, Verbose, "Found script: %s", *pathToDiscoveredItemsIndexed.full);
				}
			}

			const uint32_t max = scriptsFound.GetSize() /* Be aware! The last one is always should be the project script!*/;
			for (uint32_t i = 0; i < max; i++)
			{
				FoundScriptData& indexed = scriptsFound[i];

				Module mdl;
				if (mdl.Parse(&indexed.full))
				{
					Array<FoundScriptData> sd;
					Utils::ListDirectory(&indexed.path, sd);

					for (auto& temp : sd)
					{
						MR_LOG(LogBuildSystemFramework, Verbose, "%s script included file: %s", *mdl.moduleName, *temp.full);
					}

					loadedModules.Add(mdl);
				}
			}

			for (auto& a : loadedModules)
			{
				int j = 5345;
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

