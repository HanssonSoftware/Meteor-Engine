/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <Types/String.h>
#include <Platform/PerformanceTimer.h>
#include <Parsing/LocatingInterface.h>
//#include <shellapi.h>
#include <MemoryManager.h>
#include <FileManager.h>
#include <Parsing/ScriptParser.h>
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

	if (Commandlet::Parse("-build", temp))
	{

	}


	if (Commandlet::Parse("-source", val))
	{
		if (true)
		{
			ScriptParser solution;

			solution.OpenScript(val);
			//solution.ParseScript(solution.Chr(), ScriptParser::ParsingType::MainDescriptor);
		}

		if (Commandlet::Parse("-int", val))
		{
			//Intermediate::SearchIntermediateFiles(val);
		}

		String b;

		Solution a;
		a.Finalize(&b);

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

void BuildSystemApplication::Run()
{

}

void BuildSystemApplication::Shutdown()
{
	Application::Shutdown();
}

