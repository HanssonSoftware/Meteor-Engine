/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "GraphicsDevice.h"
#include <Types/Vector.h>
#include <thread>

IGraphicsDevice::IGraphicsDevice()
{
	graphToRender = nullptr;
}

IGraphicsDevice::~IGraphicsDevice()
{
}

bool IGraphicsDevice::Render(float deltaTime)
{
	if (!bIsDriverReady)
	{
		if (!bIsDriverInitialized)
			return false;

		return false;
	}

	return true;
}

const String IGraphicsDevice::getRendererSignatature() const
{
	return L"Unknown";
}

void IGraphicsDevice::setIsFullScreen(bool NewValue)
{ 
	bIsFullscreen = NewValue;
}

void IGraphicsDevice::resizeBuffers(Vector2<uint32> newSize)
{
	if (!bIsDriverReady)
		return;
}

void IGraphicsDevice::setDeviceReadyState(bool NewVal)
{
	bIsDriverReady = NewVal;
}

void IGraphicsDevice::setImGUIUsed(bool NewVal)
{
	bIsImGUIUsed = NewVal;
	imguiValueChange.invoke(NewVal);
}
