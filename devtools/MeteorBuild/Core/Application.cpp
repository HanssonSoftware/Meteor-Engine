/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <Types/String.h>
#include <Platform/PerformanceTimer.h>
#include <Parsing/LocatingInterface.h>
//#include <shellapi.h>
#include <MemoryManager.h>
#include <Caching/Intermediate.h>
#include <FileManager.h>
#include <Parsing/ScriptParser.h>

//#pragma comment(lib, "Shell32.lib")


BuildSystemApplication::BuildSystemApplication()
	: Application()
{
	appName = "Meteor Build";
	appNameNoSpaces = "MeteorBuild";
	appCodeName = "Apollo";

	MemoryManager::SetMinimumSize(100'000);
}

void BuildSystemApplication::Init()
{
	Application::Init();
	String val;

	if (Commandlet::Parse("-sourcedir", val))
	{
		if (ScriptParser::FindMainScript(val))
		{
			ScriptParser solution;

			solution.OpenScript(val);
			solution.ParseScript(ScriptParser::ParsingType::MainDescriptor);
		}

		if (Commandlet::Parse("-intDir", val) || Commandlet::Parse("-int", val))
		{
			Intermediate::SearchIntermediateFiles(val);
		}


		if (Commandlet::Parse("-sourcedir", val))
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

void BuildSystemApplication::Run()
{

}

void BuildSystemApplication::Shutdown()
{
	Application::Shutdown();
}

