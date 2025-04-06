/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "Window.h"
#include <Object/Object.h>
#include <Log/LogMacros.h>
#include <vector>

LOG_ADDCATEGORY(WindowManager);

class IGraphicsDevice;
class InputManager;

static constexpr const wchar_t* ApplicationClassName = L"MeteorApplication";

class WindowManager : public Object
{
	friend class IDirect3DDevice;
public:
	WindowManager();

	virtual ~WindowManager();

	virtual void Init() override;

	virtual void Destroy() override;

	virtual IWindow* createWindow(const WindowCreateInfo* CreateInfo) = 0;

	IWindow* searchFor(const String ID);

	IWindow* getFirstWindow();

	void showWindow(const String ID);

	void hideWindow(const String ID);

	bool drawAttention(const String ID);

	bool destroyWindow(const String ID);

	/** Gets the current or last focused window, can be non-engine specific. */
	virtual IWindow* getFocusedWindow() const = 0;
	
	IGraphicsDevice* getRenderContext() const { return renderContext; };

	InputManager* getInputManager() const { return inputManager; };
protected:
	IWindow* privSearchFor(const String Name);

	IGraphicsDevice* renderContext;

	InputManager* inputManager;

	std::vector<IWindow*> activeWindows; 
};

