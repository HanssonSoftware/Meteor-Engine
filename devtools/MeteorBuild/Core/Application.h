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

IMPLEMENT_APPLICATION(BuildSystemApplication);