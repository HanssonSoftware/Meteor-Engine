/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <Types/String.h>
#include <Platform/PerformanceTimer.h>
#include <Parsing/LocatingInterface.h>
#include <shellapi.h>

#pragma comment(lib, "Shell32.lib")

int main(int argc, char* argv[])
{
	return launchStranger(argc, argv);
}

BuildSystemApplication::BuildSystemApplication()
	: Application()
{

}

void BuildSystemApplication::Init()
{
	Application::Init();

	if (!ICommandlet::Parse("-sourcedir").IsEmpty())
	{
		bool b = Locator::FindAllReferences(ICommandlet::Parse("-sourcedir"));
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

