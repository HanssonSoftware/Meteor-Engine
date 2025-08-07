/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <WindowManager/Window.h>
#include <Windows/Windows.h>

class WindowsWindowManager;

class WindowsWindow : public IWindow
{
public:
	WindowsWindow(WindowsWindowManager* owner);

	virtual ~WindowsWindow() noexcept = default;

	virtual void DestroyWindow() override;

	virtual bool CreateNativeWindow(const WindowCreateInfo* windowData) override;

	virtual void ShowWindow() override;

	virtual void SetTitle(const String& newName) override;

	virtual void HideWindow() override;

	virtual void DrawAttention() override;

	virtual void* GetWindowHandle() override;

protected:
	

	HWND handle;
};

using Window = WindowsWindow;