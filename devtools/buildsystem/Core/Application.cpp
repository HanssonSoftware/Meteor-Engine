/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <string.h>


int main(int argc, char* argv[])
{
	return launchStranger(argc, argv);
}

BuildSystemApplication::BuildSystemApplication(const ApplicationInitializationInfo* Info)
	: Application(Info)
{

}

void BuildSystemApplication::Frame(float deltaTime)
{
}

void BuildSystemApplication::Init()
{
	Application::Init();

	const String sourceDirValue = ICommandlet::Get().Expected<String>("sourcedir");
	if (!sourceDirValue.isEmpty())
	{

	}
	else
	{
		MR_LOG(LogBuildSystemApplication, Fatal, TEXT("""sourcedir"" Parameter is Missing!"));
	}
}

void BuildSystemApplication::Run()
{
}

void BuildSystemApplication::Shutdown()
{
}


