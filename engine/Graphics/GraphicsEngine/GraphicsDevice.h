/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Common/Delegate.h>

#include "SceneGraph.h"
#include <Log/LogMacros.h>
#include <Types/Vector.h>
#include <thread>


LOG_ADDCATEGORY(GraphicsEngine);

typedef enum GraphicsEngineRunningState
{
	GRAPHICS_ENGINE_STATE_NONE = 0,
	GRAPHICS_ENGINE_STATE_INITIALIZING = 1,
	GRAPHICS_ENGINE_STATE_PAUSED_RENDERING = 2,
	GRAPHICS_ENGINE_STATE_RUNNING = 3,
	GRAPHICS_ENGINE_STATE_SHUTDOWN = 4,
	GRAPHICS_ENGINE_STATE_DIED = 5
} GraphicsEngineRunningState;

typedef struct Vertex
{
	Vector4<float> Pos;

	Vector4<float> Color;

	Vector3<float> Normal;
} Vertex;

class IGraphicsDevice
{
public:
	IGraphicsDevice();

	virtual ~IGraphicsDevice();

	virtual bool Render();

	virtual bool Init();

	virtual void cleanUp() = 0;

	virtual void createBuffer(void* data) = 0;

	virtual const String getRendererSignatature() const;

	virtual const bool getIsFullScreen() const { return bIsFullscreen; };

	virtual bool setIsFullScreen(bool NewValue);

	virtual void resizeBuffers(const Vector2<uint32> newSize) = 0;

	GraphicsEngineRunningState getDeviceReadyState() const { return driverState; };

	void setDeviceReadyState(GraphicsEngineRunningState NewVal);

	bool getImGUIUsed() const { return bIsImGUIUsed; };

	void setImGUIUsed(bool NewVal);

	Delegate<bool> imguiValueChange;

	SceneGraph* graphToRender;

	std::thread renderThread;

protected:
	bool bIsFullScreenSwitchEnabled = true;

	std::atomic<bool> bIsImGUIUsed = false;

	GraphicsEngineRunningState driverState;

	std::atomic<bool> bIsFullscreen = false;

	std::atomic<bool> bIsDriverInitialized = false;
};

