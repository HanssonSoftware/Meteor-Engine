/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Application.h"
#include <Types/String.h>
#include <MemoryManager.h>
#include <FileManager.h>

#include <Platform.h>

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

	//if (Commandlet::Parse("-build", temp))
	//{
	//	verb = Build;
	//}
	//else if (Commandlet::Parse("-rebuild", temp))
	//{
	//	verb = Rebuild;
	//}
	//else if (Commandlet::Parse("-gen", temp) || Commandlet::Parse("-generate", temp))
	//{
	//	verb = GenerateProjects;

void BuildSystemApplication::Run()
{
	// Application::RequestExit(0); should be added somewhere, if not the app will loop over this function

	if (!buildSystemApp.InitFramework())
	{
		MR_LOG(LogBuildSystemApplication, Fatal, "Build system error!");
	}

	Application::RequestExit(0);
}

void BuildSystemApplication::Shutdown()
{
	Application::Shutdown();
}

