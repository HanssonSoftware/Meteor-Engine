/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "BuildSystem.h"

#include <Commandlet.h>
#include <FileManager.h>

#include "Utils.h"
#include <Platform/Paths.h>
#include <Platform/Platform.h>

#include <unordered_map>

#include <PathCch.h>
#include <Application.h>

LOG_ADDCATEGORY(BuildSystemFramework);
LOG_ADDCATEGORY(Assembler);

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
				//const String combined = String::Format("%ls\\%ls", *pathToDiscoveredItemsIndexed.path, *pathToDiscoveredItemsIndexed.name);
				if (FileManager::IsEndingWith(pathToDiscoveredItemsIndexed.full, "mrbuild"))
				{
					scriptsFound.Add(pathToDiscoveredItemsIndexed);
					MR_LOG(LogBuildSystemFramework, Verbose, "Found script: %ls", *pathToDiscoveredItemsIndexed.full);
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
						MR_LOG(LogBuildSystemFramework, Verbose, "%ls module, new file added to include list: %ls", *mdl.moduleName, *temp.full);
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
	std::unordered_map<std::wstring, uint32_t> ordering;
	const uint32_t size = loadedModules.GetSize();

	for (auto& module : loadedModules)
	{
		for (auto& dependency : module.requires)
		{
			std::wstring depName = *dependency;
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
		MR_LOG(LogBuildSystemFramework, Log, "%ls (score %u)", *m.moduleName, score);
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

			PathCchRemoveFileSpec(exeDir.Data(), exeDir.Length());

			PWSTR combinedPathNonCanonicalized;
			PathAllocCombine(exeDir, intermediateLocation, PATHCCH_ALLOW_LONG_PATHS, &combinedPathNonCanonicalized);

			intermediateLocation = combinedPathNonCanonicalized;
			LocalFree(combinedPathNonCanonicalized);
		}

		const String appNameAppendedIntermediate = String::Format("%ls\\%ls", *intermediateLocation, *GetApplication()->GetApplicationCodeName()); // C:\\Meteor-Engine\\Intermediate\\Apollo
		FileManager::CreateDirectory(&appNameAppendedIntermediate);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			FileManager::DeleteDirectory(appNameAppendedIntermediate, true);		
			FileManager::CreateDirectory(&appNameAppendedIntermediate);
		}

		for (auto& module : loadedModules)
		{
			const String directoryToCreateTheFolder = String::Format("%ls\\%ls", *appNameAppendedIntermediate, *module.moduleName);
			FileManager::CreateDirectory(&directoryToCreateTheFolder);

			String projectCreationDir = String::Format("%ls\\%ls.vcxproj", *directoryToCreateTheFolder, *module.moduleName);

			HANDLE actual = CreateFileW(projectCreationDir, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (actual != INVALID_HANDLE_VALUE)
			{
				String final;
				if (module.ConstructProjectFile(&final))
				{
					ScopedPtr<char> skinnyBuffer = Platform::ConvertToNarrow(final);

					DWORD written = 0;
					if (!WriteFile(actual, skinnyBuffer.Get(), final.Length(), &written, nullptr))
					{
						MR_LOG(LogAssembler, Error, "WriteFile returned: %ls", *Platform::GetError());
					}
				}

				CloseHandle(actual);
			}
			else
			{
				MR_LOG(LogAssembler, Fatal, "Failed to create project file at: %ls", *projectCreationDir);
			}
		}

		String sourceDir;
		if (Commandlet::Parse("-source", &sourceDir))
		{
			String exeDir = Paths::GetExecutableDirctory();
			PathCchRemoveFileSpec(exeDir.Data(), exeDir.Length());

			PWSTR combinedPathNonCanonicalized;
			PathAllocCombine(exeDir, sourceDir, PATHCCH_ALLOW_LONG_PATHS, &combinedPathNonCanonicalized);

			PathCchRemoveFileSpec(combinedPathNonCanonicalized, wcslen(combinedPathNonCanonicalized));

			sourceDir = combinedPathNonCanonicalized;
			LocalFree(combinedPathNonCanonicalized);

			HANDLE solution = CreateFileW(String::Format("%ls\\%ls.slnx", *sourceDir, *ps.projectName), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (solution != INVALID_HANDLE_VALUE)
			{
				String buffer;
				if (ps.Finalize(&buffer))
				{
					ScopedPtr<char> skinnyBuffer = Platform::ConvertToNarrow(buffer);

					DWORD written = 0;
					if (!WriteFile(solution, skinnyBuffer.Get(), buffer.Length(), &written, nullptr))
					{
						MR_LOG(LogAssembler, Error, "WriteFile returned: %ls", *Platform::GetError());
					}
				}

				CloseHandle(solution);
				return true;
			}
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

