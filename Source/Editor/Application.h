/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#pragma once
#include <Application/Application.h>
#include <Types/Vector.h>

LOG_ADDCATEGORY(EditorApplication);

class EditorApplication : public Application
{
public:
	EditorApplication() {};

	virtual void Frame() override;

	virtual void Init() override;
}; 


int launchStranger(int ArgumentCount, const char* Arguments[])
{
	WindowCreateInfo wInfo = {};
	wInfo.size = Vector2<uint32>(1280, 720);
	//wInfo.windowName = L"wind";

	ApplicationInitializationInfo appInfo = {};
	appInfo.appName = TEXT("MeteorEditor");
	appInfo.windowCreateInfo = &wInfo;

	instantiateApp<EditorApplication>(&appInfo);

	return 68;
}
