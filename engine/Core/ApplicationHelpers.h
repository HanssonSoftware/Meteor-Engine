/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Platform/WindowManager/WindowManager.h>

enum ApplicationFlags
{
	APPFLAG_NO_WINDOW = 1 << 0,
	APPFLAG_HIDE_CONSOLE_DURING_DEBUG = 1 << 1,
	APPFLAG_NO_FULLSCREEN = 1 << 2,
	APPFLAG_DISPLAY_QUICK_INFO_ABOUT_MEMORY_USAGE = 1 << 3,
	APPFLAG_NO_FILE_LOGGING = 1 << 4,
	APPFLAG_ENABLE_VERBOSE_LOGGING = 1 << 5,
	APPFLAG_START_ONLY_FROM_COMMAND_LINE = 1 << 6
};

struct ApplicationInitializationInfo
{
	String appName;

	String appCodeName;

	WindowCreateInfo windowCreateInfo;

	String arguments;

	int flags = 0;

	float requiredMinimumMemoryInPercent = 0.f;
};

enum ApplicationState
{
	APPLICATIONSTATE_INITIALIZATION,
	APPLICATIONSTATE_RUNNING,
	APPLICATIONSTATE_RESTARTING,
	APPLICATIONSTATE_SHUTDOWN

};
