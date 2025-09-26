/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <Types/String.h>
#include <Platform/PerformanceTimer.h>
#include <Parsing/LocatingInterface.h>
//#include <shellapi.h>
#include <MemoryManager.h>
#include <FileManager.h>
#include <Parsing/ModuleProcessor.h>
#include <Parsing/Solution.h>

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
	
	String val, temp;

	RedirectRoutingVerb(temp);

	if (Commandlet::Parse("-source", val))
	{
		String mainScriptLocation;
		if (Locator::FindMainScript(val))
		{
			Module* topDirectoryScript = Module::CreateModule(val);
		}

		if (Commandlet::Parse("-source", val))
		{
			Array<String> pathToDiscoveredItems;
			Locator::LocateSources(val, pathToDiscoveredItems);
		}
		else
		{

		}
	}
	else
	{
		MR_LOG(LogBuildSystemApplication, Fatal, "No source directory passed!");
	}
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
}

void BuildSystemApplication::Run()
{

}

void BuildSystemApplication::Shutdown()
{
	Application::Shutdown();
}

