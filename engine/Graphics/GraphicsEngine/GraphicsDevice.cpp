/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "GraphicsDevice.h"
#include <Types/Vector.h>
#include <thread>
#include <Application/Application.h>

IGraphicsDevice::IGraphicsDevice()
{
	graphToRender = nullptr;
}

IGraphicsDevice::~IGraphicsDevice()
{
}

bool IGraphicsDevice::Render()
{
	if (driverState == GRAPHICS_ENGINE_STATE_RUNNING)
	{
		return true;
	}

	return false;
}

bool IGraphicsDevice::Init()
{
	setDeviceReadyState(GRAPHICS_ENGINE_STATE_INITIALIZING);
	bIsFullScreenSwitchEnabled = !((Application::Get()->getAppInfo()->flags & APPFLAG_NO_FULLSCREEN));

	return true;
}

const String IGraphicsDevice::getRendererSignatature() const
{
	return L"Unknown";
}

bool IGraphicsDevice::setIsFullScreen(bool NewValue)
{
	if (!bIsFullScreenSwitchEnabled)
		return false;
	
	bIsFullscreen = NewValue;
	return true;
}

void IGraphicsDevice::setDeviceReadyState(GraphicsEngineRunningState NewVal)
{
	driverState = NewVal;
}

void IGraphicsDevice::setImGUIUsed(bool NewVal)
{
	bIsImGUIUsed = NewVal;
	imguiValueChange.invoke(NewVal);
}
