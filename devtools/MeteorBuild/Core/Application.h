/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Application.h>
#include <Logging/LogMacros.h>
#include <Platform/WindowManager/WindowManager.h>
#include <Commandlet.h>

LOG_ADDCATEGORY(BuildSystemApplication);

struct BuildSystemApplication : public Application 
{
public:
	BuildSystemApplication();

	virtual void Init() override;

	virtual void Run() override;

	virtual void Shutdown() override;
};

int launchStranger(int ArgumentCount, char* Arguments[])
{
	ApplicationInitializationInfo appInfo = {};
	appInfo.appName = String("MeteorBuild");
	appInfo.appCodeName = "Hansson";
	appInfo.requiredMinimumMemoryInPercent = 0.03f;
	appInfo.flags = APPFLAG_NO_WINDOW | APPFLAG_NO_FILE_LOGGING | APPFLAG_START_ONLY_FROM_COMMAND_LINE;

	ICommandlet::Initalize();

	BuildSystemApplication newBuilderApp = {};
	Application::InstantiateApplication(&newBuilderApp, &appInfo);

	return Application::GetRequestExitCode();
}
