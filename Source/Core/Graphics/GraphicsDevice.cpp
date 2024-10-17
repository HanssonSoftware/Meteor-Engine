/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#include "GraphicsDevice.h"
#include <Types/Vector.h>
#include <thread>

bool IGraphicsDevice::render()
{
	if (!bIsDriverReady)
	{
		if (!bIsDriverInitialized)
			return false;

		return false;
	}

	return true;
}

void IGraphicsDevice::updateSize(Vector2<uint32> newSize)
{
	if (!bIsDriverReady)
	{
		if (bIsDriverInitialized) {
			MR_LOG(LogGraphicsEngine, Warn, TEXT("Driver is not ready! %s is called before ready, you can ignore this, if you know what to do."), __FUNCTIONW__);
		}

		return;
	}
}

const std::wstring IGraphicsDevice::getRendererSignatature() const
{
	return L"Unknown";
}

void IGraphicsDevice::setIsFullScreen(bool NewValue)
{ 
	bIsFullscreen = NewValue;
}

void IGraphicsDevice::setDriverReadyState(bool NewVal)
{
	bIsDriverReady = NewVal;
}
