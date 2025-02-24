/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Application.h"
#include <string.h>
#include "../Parsing/DirectoryExplorer.h"
#include "../Parsing/SolutionGenerator.h"


int main(int argc, char* argv[])
{
	return launchStranger(argc, argv);
}

BuildSystemApplication::BuildSystemApplication(const ApplicationInitializationInfo* Info)
	: Application(Info)
{

}

void BuildSystemApplication::Init()
{
	Application::Init();

	String sourceValue = ICommandlet::Get().Expected<String>("sourcedir");
	if (!sourceValue.isEmpty())
	{
		DirectoryExplorer dxp;
		dxp.startExpedition(sourceValue);

		String slnValue = ICommandlet::Get().Expected<String>("makesln");
		if (!slnValue.isEmpty())
		{
			SolutionGenerator sln(slnValue);

			dxp.processModules();
			sln.getHandle()->isPersistent() ? sln.readSolution() : sln.startSolutionFill();

			int hd = 5;
		}
	}
	else
	{
		MR_LOG(LogBuildSystemApplication, Fatal, TEXT("""sourcedir"" Parameter is Missing! Without This We are Unable to Find Source Directory!"));
	}
}

void BuildSystemApplication::Run()
{

}

void BuildSystemApplication::Shutdown()
{
}


