/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#include <Types/PlatformDefs.h>
#include <Types/Vector.h>

class Visual
{
public:
	virtual void createWidget(Vector2<uint32> size);

	virtual void Init();

	virtual void Update(float deltaTime);
};

