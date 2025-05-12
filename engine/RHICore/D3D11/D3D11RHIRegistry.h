/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <RHI/RHIRegistry.h>

class D3D11RHIRegistry : public IRHIRegistry
{
public:
	virtual int GetMonitorIndex() const override;

	virtual String GetName() const override;

	virtual bool Initialize() override;

	virtual void CleanUp() const override;
};

