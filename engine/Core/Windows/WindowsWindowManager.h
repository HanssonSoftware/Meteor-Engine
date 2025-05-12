/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/WindowManager.h>
#include <Windows/Windows.h>

class WindowsWindow;

class WindowsWindowManager : public IWindowManager
{
public:
	virtual ~WindowsWindowManager();

	virtual IWindow* CreateNativeWindow(const WindowCreateInfo* CreateInfo) override;

	virtual IWindow* GetFocusedWindow() override;

	virtual WindowsWindow* SearchForHWND(const HWND hWnd);

	HINSTANCE GetInstance() const { return instance; }

	bool GetIsUsingFallbackClass() const { return bIsUsingFallbackClassName; };
private:
	inline bool RegisterWindowClass();

	bool bIsWinAPIClassRegistered = false;

	bool bIsUsingFallbackClassName = false;

	HINSTANCE instance;
};

LRESULT CALLBACK MeteorSpecifiedWindowProcedure(HWND, UINT, WPARAM, LPARAM);

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);