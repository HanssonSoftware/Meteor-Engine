/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Application.h>
#include <Types/Vector.h>
#include <Platform/WindowManager/WindowManager.h>
#include <Commandlet.h>
//#include <Common/Pointers.h>

LOG_ADDCATEGORY(EditorApplication);

class EditorApplication : public Application
{
public:
	EditorApplication();

	virtual void Init() override;

	virtual void Run() override;

	virtual void Shutdown() override;
}; 


int launchStranger(int ArgumentCount, char* Arguments[])
{
	WindowCreateInfo wInfo = {};
	wInfo.size = Vector2<>(1280, 720);
	wInfo.windowID = String("Super");
	wInfo.windowName = String("Meteor Editor");

	ApplicationInitializationInfo appInfo = {};
	appInfo.appName = "Meteor Editor";
	appInfo.appCodeName = "Astronaut";
	appInfo.windowCreateInfo = wInfo;
	appInfo.flags = APPFLAG_ENABLE_VERBOSE_LOGGING;

	ICommandlet::Initalize();

	EditorApplication newEditorApp = {};
	App::InstantiateApplication(&newEditorApp, &appInfo);

	return App::GetRequestExitCode();
}
