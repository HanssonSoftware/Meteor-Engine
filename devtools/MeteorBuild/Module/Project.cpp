/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Project.h"
#include <Core/Utils.h>
#include <Platform/FileManager.h>
#include <Platform/File.h>

#include "Module.h"
#include <Core/Application.h>

LOG_ADDCATEGORY(ProjectScriptParser);

bool Project::Finalize(String* output)
{
	if (BuildSystemApplication* app = GetApplication<BuildSystemApplication>())
	{
		const Array<Module>* modules = app->GetBuildSystem().GetModules();

		String project;
		for (auto& mdl : *modules)
		{
			String actualProject = String::Format(L"\t\t<Project Path = \"%ls\" Id=\"%ls\"/>\n", *mdl.generatedProjectFile, *mdl.identification);
			project = String::Format(L"%ls%ls", *project, *actualProject);
		}

		*output = String::Format(L"<!-- This file is generated with MeteorBuild(R) -->\n"
			L"<Solution>\n"
			L"\t<Configurations>\n"
			L"\t\t<BuildType Name=\"Debug\" />\n"
			L"\t\t<BuildType Name=\"Shipping\" />\n"
			L"\t\t<Platform Name = \"x64\" />\n"
			L"\t</Configurations>\n"
			L"\t<Folder Name = \"/Modules/\">\n"
			L"%ls"
			L"\t</Folder>\n"
			L"</Solution>", 
			*project);

		return true;
	}

	return false;
}
