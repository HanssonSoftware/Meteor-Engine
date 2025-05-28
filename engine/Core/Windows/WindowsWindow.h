/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/WindowManager/Window.h>

class WindowsWindowManager;

class WindowsWindow : public IWindow
{
public:
	WindowsWindow(WindowsWindowManager* owner);

	virtual void DestroyWindow() override;

	virtual bool CreateNativeWindow(const WindowCreateInfo* windowData) override;

	virtual void ShowWindow() override;

	virtual void SetTitle(const String newName) override;

	virtual void HideWindow() override;

	virtual void DrawAttention() override;

	virtual void* GetWindowHandle() override;
};

