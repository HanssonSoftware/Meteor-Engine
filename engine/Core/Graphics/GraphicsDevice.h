/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Common/Delegate.h>

#include "SceneGraph.h"
#include <Log/LogMacros.h>
#include <Types/Vector.h>


LOG_ADDCATEGORY(GraphicsEngine);

enum ESelectableDrivers
{
	None,
	OpenGL,
	Vulkan,
	D3D11
};

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

	virtual bool Render(float deltaTime);

	virtual bool Init() = 0;

	virtual void cleanUp() = 0;

	virtual const String getRendererSignatature() const;

	virtual const bool getIsFullScreen() const { return bIsFullscreen; };

	virtual void setIsFullScreen(bool NewValue);

	virtual void resizeBuffers(Vector2<uint32> newSize);

	bool getDeviceReadyState() const { return bIsDriverReady; };

	void setDeviceReadyState(bool NewVal);

	bool getImGUIUsed() const { return bIsImGUIUsed; };

	void setImGUIUsed(bool NewVal);

	Delegate<bool> imguiValueChange;

	SceneGraph* graphToRender;
protected:
	std::atomic<bool> bIsImGUIUsed = false;

	std::atomic<bool> bIsDriverReady = false;

	std::atomic<bool> bIsFullscreen = false;

	std::atomic<bool> bIsDriverInitialized = false;
};

