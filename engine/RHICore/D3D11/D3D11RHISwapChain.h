/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include <RHI/RHISwapChain.h>

struct IDXGISwapChain;

#pragma once
class D3D11RHISwapChain : public IRHISwapChain
{
public:

	virtual void Present() override;

	operator IDXGISwapChain*()
	{
		return object;
	}

private:
	IDXGISwapChain* object;
};

