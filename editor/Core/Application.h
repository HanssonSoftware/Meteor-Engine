/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Application/Application.h>
#include <Types/Vector.h>
#include <Window/WindowManager.h>
#include <Application/Commandlet.h>

LOG_ADDCATEGORY(EditorApplication);

class EditorApplication : public Application
{
public:
	EditorApplication(const ApplicationInitializationInfo* Info);

	virtual void Frame(float deltaTime) override;

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
	appInfo.appName = String("MeteorEditor");
	appInfo.windowCreateInfo = &wInfo;

	ICommandlet::Get().Parse(ArgumentCount, Arguments);

	instantiateApp<EditorApplication>(&appInfo);

	return 0;
}
