/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "BuildSystem.h"

#include <Commandlet.h>
#include <Parsing/Finder.h>
#include <FileManager.h>

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

		for (auto& pathToDiscoveredItemsIndexed : pathToDiscoveredItems)
		{
			if (FileManager::IsEndingWith(pathToDiscoveredItemsIndexed, "mrbuild"))
			{
				scripts.Add(pathToDiscoveredItemsIndexed);
				MR_LOG(LogBuildSystemFramework, Log, "Found: %s", *pathToDiscoveredItemsIndexed);
			}
		}
	}
}

void BuildSystem::ParseDescriptorScript()
{
	IFile* slnDescriptor = FileManager::CreateFileOperation(&scripts[scripts.GetSize()], FileAccessMode::OPENMODE_READ, FileShareMode::SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS);
	if (slnDescriptor != nullptr)
	{
		slnDescriptor->Read();

		return;
	}
}
