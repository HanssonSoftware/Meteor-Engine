/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "BuildSystem.h"

#include <Commandlet.h>
#include <FileManager.h>

#include "Utils.h"
#include <Platform/Paths.h>
#include <Platform/Platform.h>

#include <unordered_map>

#include <PathCch.h>

LOG_ADDCATEGORY(BuildSystemFramework);

bool BuildSystem::InitFramework()
{
	if (ReadArguments())
	{
		bool bHasProject = false, bAtLeastOneScriptParsed = false;
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
				FoundScriptData indexed = scriptsFound[i];

				Module mdl;
				if (mdl.Parse(&indexed.full))
				{
					if (!bAtLeastOneScriptParsed) bAtLeastOneScriptParsed = true;

					Array<FoundScriptData> sd;
					Utils::ListDirectory(&indexed.path, sd);

					for (auto& temp : sd)
					{
						mdl.files.Add(indexed.full);
						MR_LOG(LogBuildSystemFramework, Verbose, "%s script included file: %s", *mdl.moduleName, *temp.full);
					}

					loadedModules.Add(mdl);
				}
				else
				{
					if (ps.Parse(&indexed.full))
						bHasProject = true;
				}
			}
		}

		return bHasProject && bAtLeastOneScriptParsed;
	}


	return false;
}

/** WARNING! This function uses, std::string from C++ standard. */
void BuildSystem::OrderModules()
{
	std::unordered_map<std::string, uint32_t> ordering;
	const uint32_t size = loadedModules.GetSize();

	for (auto& module : loadedModules)
	{
		for (auto& dependency : module.requires)
		{
			std::string depName = *dependency;
			ordering[depName]++;
		}
	}

	struct Pair
	{
		uint32_t score;
		uint32_t index;
	};

	Array<Pair> scores;
	scores.Resize(size);

	for (uint32_t i = 0; i < size; ++i)
	{
		auto& m = loadedModules[i];
		auto it = ordering.find(*m.moduleName);
		uint32_t score = (it != ordering.end()) ? it->second : 0;
		scores[i].score = score;
		scores[i].index = i;
	}

	for (uint32_t i = 0; i < size; ++i)
	{
		for (uint32_t j = i + 1; j < size; ++j)
		{
			if (scores[j].score > scores[i].score)
			{
				auto tmp = scores[i];
				scores[i] = scores[j];
				scores[j] = tmp;
			}
		}
	}

	Array<Module> sorted;
	sorted.Resize(size);

	for (uint32_t k = 0; k < size; ++k)
	{
		sorted[k] = loadedModules[scores[k].index];
	}

	loadedModules = std::move(sorted);

	for (uint32_t i = 0; i < size; ++i)
	{
		auto& m = loadedModules[i];
		auto it = ordering.find(*m.moduleName);
		uint32_t score = (it != ordering.end()) ? it->second : 0;
		MR_LOG(LogBuildSystemFramework, Log, "%s (score %u)", *m.moduleName, score);
	}
}

bool BuildSystem::BuildProjectFiles()
{
	String intermediateLocation;
	if (loadedModules.GetSize() > 0 && Commandlet::Parse("-int", &intermediateLocation))
	{
		if (FileManager::IsPathRelative(&intermediateLocation))
		{
			String exeDir = Paths::GetExecutableDirctory();

			ScopedPtr<wchar_t> exeDirWide = Platform::ConvertToWide(exeDir);
			PathCchRemoveFileSpec(exeDirWide.Get(), wcslen(exeDirWide));

			ScopedPtr<wchar_t> nameWide = Platform::ConvertToWide(*intermediateLocation);

			PWSTR combinedPathNonCanonicalized;
			PathAllocCombine(exeDirWide, nameWide, PATHCCH_ALLOW_LONG_PATHS, &combinedPathNonCanonicalized);

			intermediateLocation = combinedPathNonCanonicalized;
			LocalFree(combinedPathNonCanonicalized);
		}


		ScopedPtr<wchar_t> fullPath = Platform::ConvertToWide(intermediateLocation);



		FileManager::CreateDirectory(intermediateLocation, true);

		const uint32_t size = loadedModules.GetSize();
		for (uint32_t i = 0; i < size; i++)
		{

		}
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

