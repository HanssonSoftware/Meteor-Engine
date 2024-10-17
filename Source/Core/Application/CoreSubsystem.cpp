/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#include "CoreSubsystem.h"

CoreSubsystem& CoreSubsystem::Get()
{
	static CoreSubsystem instance;
	return instance;
}

void CoreSubsystem::tickSystems(float deltaTime)
{
	for (auto& Subsystem : systems)
	{
		Subsystem.tick(deltaTime);
	}
}
