/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Core/Application.h>
#include <Types/Vector.h>
#include <Platform/WindowManager.h>
#include <Commandlet.h>
#include <Common/Pointers.h>

LOG_ADDCATEGORY(EditorApplication);

class EditorApplication : public Application
{
public:
	EditorApplication(const ApplicationInitializationInfo* Info);

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
	appInfo.appName = "MeteorEditor";
	appInfo.windowCreateInfo = wInfo;
	appInfo.flags = APPFLAG_ENABLE_VERBOSE_LOGGING;

	ICommandlet::Get().Parse(ArgumentCount, Arguments);

	ScopedPtr<EditorApplication> App(new EditorApplication(&appInfo));
	App->Init();

	return 0;
}
