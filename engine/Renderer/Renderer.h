/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Module/Module.h>

class Renderer : public Module
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	virtual void Update() override;

	static Renderer* InitModule(const String& name);
};


//IMPLEMENT_MODULE(Renderer);