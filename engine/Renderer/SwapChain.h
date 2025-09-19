/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Vector.h>
#include <stdint.h>

class IRHISwapChain
{
public:
	IRHISwapChain() = default;

	virtual ~IRHISwapChain() = default;

	virtual void Init() = 0;

	virtual void Shutdown() = 0;

	virtual void Present() = 0;
protected:
	enum { WINDOWED, FULLSCREEN, EXCLUSIVE_FULLSCREEN } swapChainMode;

	Vector2<uint32_t> windowSize;
};

