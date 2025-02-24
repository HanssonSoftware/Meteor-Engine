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

enum WindowFlags : sint8
{
	 WF_HIDDENDEFAULT = 1 << 0,
	 WF_SHOWNDEFAULT = 1 << 1

};

class WindowManager : public Object
{
	friend class IDirect3DDevice;
public:
	WindowManager();

	virtual ~WindowManager();

	static WindowManager& Get();

	Window* createWindow(const WindowCreateInfo* CreateInfo);

	Window* searchFor(const String ID);

	Window* getFirstWindow();

	void showWindow(const String ID);

	void hideWindow(const String ID);

	bool drawAttention(const String ID);

	bool destroyWindow(const String ID);

	/** Gets the current or last focused window, can be non-engine specific. */
	Window* getFocusedWindow() const;
	
	IGraphicsDevice* getRenderContext() const { return renderContext; };

	InputManager* getInputManager() const { return inputManager; };
private:
#ifdef _WIN32
	inline bool registerWindowClass();
#endif
	Window* privSearchFor(const String Name);

	IGraphicsDevice* renderContext;

	InputManager* inputManager;

	std::vector<Window*> activeWindows; 
#ifdef _WIN32
	bool bIsWinAPIClassRegistered = false;
#endif
};

