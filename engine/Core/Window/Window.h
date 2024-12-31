/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
//#include "WindowManager.h"
#include <Platform/Microsoft/MinWin.h>
#include <Types/Vector.h>

struct WindowCreateInfo;
class String;

class Window
{
	friend class WindowManager;

	Window(WindowManager* newOwner);

	bool createWindow(const WindowCreateInfo* CreateInfo);
public:
	~Window();

	Vector2<uint32> getSize() const;

	void destroyWindow();

	void showWindow();

	void setTitle(String newName);

	void hideWindow();

	constexpr const int evaluateFlags(int Flags) noexcept;

	void* getWindowHandle();

	const WindowCreateInfo* windowData;
private:
	WindowManager* owner;

	void* Handle = nullptr;
};

