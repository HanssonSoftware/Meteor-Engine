/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "Window.h"
#include <Object/Object.h>
#include <Logging/LogMacros.h>
#include <vector>

LOG_ADDCATEGORY(WindowManager);

class IRHIRegistry;
class InputManager;

static constexpr const wchar_t* ApplicationClassName = L"MeteorApplication";

class IWindowManager
{
public:
	IWindowManager();

	virtual ~IWindowManager() = default;

	virtual void Init();

	virtual void Shutdown();

	virtual IWindow* CreateNativeWindow(const WindowCreateInfo* CreateInfo) = 0;

	IWindow* SearchFor(const String ID);

	IWindow* GetFirstWindow();

	void ShowWindow(const String ID);

	void HideWindow(const String ID);

	bool DrawAttention(const String ID);

	bool DestroyWindow(const String ID);

	/** Gets the current or last focused window, can be non-engine specific. */
	virtual IWindow* GetFocusedWindow() = 0;
	
	IRHIRegistry* GetRenderContext() const { return renderContext; };

	InputManager* GetInputManager() const { return inputManager; };
protected:
	IWindow* privSearchFor(const String Name);

	IRHIRegistry* renderContext;

	InputManager* inputManager;

	std::vector<IWindow*> activeWindows; 
};

