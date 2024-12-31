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

typedef enum WindowFlags : sint8
{
	 WF_HIDDENDEFAULT = 1 << 0,
	 WF_SHOWNDEFAULT = 1 << 1

} WindowFlags;

typedef struct WindowCreateInfo
{
	/** Constant ID, good for searching window.*/
	String windowID;

	/** Distinguish between the ID, the ID is for searching purposes.*/
	String windowName;

	Vector2<uint32> size;

	int flags = -1;

} WindowCreateInfo;


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

	bool destroyWindow(const String ID);

	IGraphicsDevice* getRenderContext() const { return renderContext; };

	InputManager* getInputManager() const { return inputManager; };
private:
#ifdef _WIN32
	inline bool registerWindowClass();
#endif
	IGraphicsDevice* renderContext;

	InputManager* inputManager;

	std::vector<Window*> activeWindows;
#ifdef _WIN32
	bool bIsWinAPIClassRegistered = false;
#endif
};

