/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/Vector.h>

class IRHISwapChain
{
public:

	IRHISwapChain() = default;

	virtual ~IRHISwapChain() noexcept = default;

	virtual void Init();

	virtual void Shutdown();

	virtual void Present() = 0;

	virtual Vector2<float> GetSize() const noexcept;

protected:
	bool bIsUsingVsync = false;
};

