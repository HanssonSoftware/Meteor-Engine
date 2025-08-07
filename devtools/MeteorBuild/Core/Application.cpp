/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <Types/String.h>
#include <Platform/PerformanceTimer.h>
#include <Parsing/LocatingInterface.h>
#include <shellapi.h>
#include <MemoryManager.h>
#include <Caching/Intermediate.h>

#pragma comment(lib, "Shell32.lib")


BuildSystemApplication::BuildSystemApplication()
	: Application()
{
	MemoryManager::SetMinimumSize(1'000'000);
}

void BuildSystemApplication::Init()
{
	Application::Init();
	String val;

	if (!Commandlet::Parse("-sourcedir", val))
	{
		bool b = Locator::FindAllReferences(val);
	}
	else
	{
		MR_LOG(LogBuildSystemApplication, Fatal, """-sourcedir"" Parameter is Missing! Unable to find source directory!");
	}
}

void BuildSystemApplication::Run()
{

}

void BuildSystemApplication::Shutdown()
{
	Application::Shutdown();
}

