/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

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

	String workDirectory;
};

int launchStranger(int ArgumentCount, char* Arguments[])
{
	ApplicationInitializationInfo appInfo = {};
	appInfo.appName = String("MeteorBuild");
	appInfo.appCodeName = "Hansson";
	appInfo.flags = APPFLAG_NO_WINDOW | APPFLAG_NO_FILE_LOGGING | APPFLAG_START_ONLY_FROM_COMMAND_LINE;

	ICommandlet::Initalize();

	BuildSystemApplication newBuilderApp = {};
	App::InstantiateApplication(&newBuilderApp, &appInfo);

	return App::GetRequestExitCode();
}
