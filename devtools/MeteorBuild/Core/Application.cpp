/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <Types/String.h>
#include <Platform/PerformanceTimer.h>
#include <Parsing/Finder.h>
//#include <shellapi.h>
#include <MemoryManager.h>
#include <FileManager.h>
#include <Parsing/ModuleProcessor.h>
//#include <Parsing/SolutionDescriptor.h>

#include <Platform.h>
#include <Parsing/Solution.h>
#include <Parsing/SolutionDescriptor.h>

//#pragma comment(lib, "Shell32.lib")


BuildSystemApplication::BuildSystemApplication()
	: Application()
{
	appName = "Meteor Build";
	appNameNoSpaces = "MeteorBuild";
	appCodeName = "Apollo";

	Memory.memoryReserveInBytes = 100'000;
}

void BuildSystemApplication::Init()
{
	Application::Init();
}

void BuildSystemApplication::RedirectRoutingVerb(String& temp)
{
	if (Commandlet::Parse("-build", temp))
	{
		verb = Build;
	}
	else if (Commandlet::Parse("-rebuild", temp))
	{
		verb = Rebuild;
	}
	else if (Commandlet::Parse("-gen", temp) || Commandlet::Parse("-generate", temp))
	{
		verb = GenerateProjects;
	}
}

void BuildSystemApplication::Run()
{
	// Application::RequestExit(0); should be added somewhere, if not the app will loop over this function
	
	String val, temp;

	RedirectRoutingVerb(temp);

	if (Commandlet::Parse("-source", val))
	{
		if (Finder::FindMainScript(val))
		{
			Solution sln;
			
			SolutionDescriptor::OpenProject(val);

			//Module* topDirectoryScript = Module::CreateModule(val);
		}

		if (Commandlet::Parse("-source", val))
		{
			Array<String> pathToDiscoveredItems;
			Finder::LocateSources(val, pathToDiscoveredItems);
		}
		else
		{

		}

		Application::RequestExit(0);
	}
	else
	{
		MR_LOG(LogBuildSystemApplication, Fatal, "No source directory passed!");
	}
}

void BuildSystemApplication::Shutdown()
{
	Application::Shutdown();
}

