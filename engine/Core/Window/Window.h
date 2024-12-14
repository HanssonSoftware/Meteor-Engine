/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/Microsoft/MinWin.h>
#include <Types/Vector.h>

struct WindowCreateInfo;

class Window
{
	friend class WindowManager;

	Window();

	bool createWindow(const WindowCreateInfo* CreateInfo);
public:
	~Window();

	Vector2<uint32> getSize() const;

	void destroyWindow();

	void showWindow();

	void hideWindow();

	constexpr const int evaluateFlags(int Flags) noexcept;

	void* getWindowHandle();

	const WindowCreateInfo* windowData;
private:
	void* Handle = nullptr;
};

