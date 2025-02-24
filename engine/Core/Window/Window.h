/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
//#include "WindowManager.h"
#include <Platform/Microsoft/MinWin.h>
#include <Types/String.h>
#include <Types/Vector.h>


struct WindowCreateInfo
{
	/** Constant ID, good for searching window.*/
	String windowID;

	/** Distinguish between the ID, the ID is for searching purposes.*/
	String windowName;

	Vector2<uint32> size = 0;

	int flags = -1;

};


class Window
{
	friend class WindowManager;

	Window(WindowManager* newOwner);

	bool createWindow(const WindowCreateInfo* CreateInfo);
public:
#ifdef _WIN32
	Window(const HWND wnd);
#endif
	~Window();

	Vector2<uint32> getSize() const;

	void destroyWindow();

	void showWindow();

	void setTitle(const String newName);

	void hideWindow();

	bool drawAttention();

	constexpr const int evaluateFlags(int Flags) noexcept;

	void* getWindowHandle();

	WindowCreateInfo windowData;
private:
	WindowManager* owner;

	void* Handle = nullptr;
};

