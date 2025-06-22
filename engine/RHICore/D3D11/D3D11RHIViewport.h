/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <RHI/RHIViewport.h>

class D3D11RHIViewport : public IRHIViewport
{
public:
	D3D11RHIViewport() = delete;

	D3D11RHIViewport(IRHIRegistry* owningRegistry, const uint32 sizeX, const uint32 sizeY, const uint32 posX, const uint32 posY);

	virtual void Init() override;

	virtual void CleanUp() override;
};

