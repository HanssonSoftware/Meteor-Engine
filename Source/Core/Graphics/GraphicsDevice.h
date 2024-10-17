/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#pragma once
#include <string>
#include <Application/Subsystem.h>
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
	Vector3<float> Pos;

	Vector4<float> Color;
} Vertex;

class IGraphicsDevice : public Subsystem
{
public:
	IGraphicsDevice() { };

	virtual bool render();

	virtual void init() = 0;

	virtual void updateSize(Vector2<uint32> newSize);

	virtual void cleanUp() = 0;

	virtual const std::wstring getRendererSignatature() const;

	virtual const bool getIsFullScreen() const { return bIsFullscreen; };

	virtual void setIsFullScreen(bool NewValue);

	bool getDriverReadyState() const { return bIsDriverReady; };

	void setDriverReadyState(bool NewVal);

protected:
	std::atomic<bool> bIsDriverReady = false;

	std::atomic<bool> bIsFullscreen = false;

	std::atomic<bool> bIsDriverInitialized = false;
};

