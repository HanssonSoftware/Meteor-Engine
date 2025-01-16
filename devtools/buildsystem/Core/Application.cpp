/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <string.h>
#include "../Parsing/DirectoryExplorer.h"


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

static String sourceValue;
void BuildSystemApplication::Init()
{
	Application::Init();

	sourceValue = ICommandlet::Get().Expected<String>("sourcedir");
	if (!sourceValue.isEmpty())
	{
		DirectoryExplorer dxp;
		dxp.startExpedition(sourceValue);
		size_t jkd = dxp.modules.size();
	}
	else
	{
		MR_LOG(LogBuildSystemApplication, Fatal, TEXT("""sourcedir"" Parameter is Missing, This Must be Appended to Find the Source Directory!"));
	}
}

void BuildSystemApplication::Run()
{
}

void BuildSystemApplication::Shutdown()
{
}


