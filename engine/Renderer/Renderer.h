/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Module/Module.h>

class Renderer : public Module
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};


IMPLEMENT_MODULE(Renderer);