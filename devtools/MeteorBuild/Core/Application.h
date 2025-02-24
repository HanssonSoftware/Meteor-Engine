/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Core/Application/Application.h>
#include <Log/LogMacros.h>
#include <Core/Window/WindowManager.h>
#include <Core/Application/Commandlet.h>

LOG_ADDCATEGORY(BuildSystemApplication);

class BuildSystemApplication : public Application 
{
public:
	BuildSystemApplication(const ApplicationInitializationInfo* Info);

	virtual void Init() override;

	virtual void Run() override;

	virtual void Shutdown() override;

	String workDirectory;
};

int launchStranger(int ArgumentCount, char* Arguments[])
{
	ApplicationInitializationInfo appInfo = {};
	appInfo.appName = String("MeteorBuild");
	appInfo.flags = APPFLAG_NO_WINDOW | APPFLAG_NO_FILE_LOGGING;

	ICommandlet::Get().Parse(ArgumentCount, Arguments);
	BuildSystemApplication* App = new BuildSystemApplication(&appInfo);

	App->Init();

	delete App;
	return 0;
}
