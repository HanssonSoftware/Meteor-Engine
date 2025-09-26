/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Application.h>
#include <Logging/LogMacros.h>
#include <Platform/WindowManager/WindowManager.h>
#include <Commandlet.h>

LOG_ADDCATEGORY(BuildSystemApplication);

enum ECurrentlyUsedVerb { Build, Rebuild };

struct BuildSystemApplication : public Application 
{
public:
	BuildSystemApplication();

	virtual void Init() override;

	void RedirectRoutingVerb(String& temp);

	virtual void Run() override;

	virtual void Shutdown() override;

	ECurrentlyUsedVerb GetUsedVerb() const { return verb; }

protected:
	ECurrentlyUsedVerb verb;
};

IMPLEMENT_APPLICATION(BuildSystemApplication);