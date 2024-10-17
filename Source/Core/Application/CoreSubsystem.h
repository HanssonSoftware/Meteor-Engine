/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#include <vector>
#include "Subsystem.h"

class CoreSubsystem
{
	
public:
	static CoreSubsystem& Get();

	inline void tickSystems(float deltaTime);

	std::vector<Subsystem> systems;
};

	