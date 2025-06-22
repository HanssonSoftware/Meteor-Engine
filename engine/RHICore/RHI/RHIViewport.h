/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/PlatformDefs.h>

class IRHIRegistry;

class IRHIViewport
{
public:
	IRHIViewport() = delete;

	IRHIViewport(IRHIRegistry* owningRegistry, const uint32 sizeX, const uint32 sizeY, const uint32 posX, const uint32 posY);

	virtual ~IRHIViewport() noexcept = default;

	virtual void Init() = 0;

	virtual void CleanUp() = 0;

	void SetVisibility(bool newVal) { bIsVisible = newVal; }

	bool GetVisibility() const { return bIsVisible; }
protected:
	IRHIRegistry* registry;

	uint32 sizeX = 0;

	uint32 sizeY = 0;

	uint32 posX = 0;

	uint32 posY = 0;

	bool bIsVisible = false;
};

