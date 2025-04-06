/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Window/WindowManager.h>

class WinWindowManager : public WindowManager
{
public:
	virtual IWindow* createWindow(const WindowCreateInfo* CreateInfo) override;

private:
	inline bool registerWindowClass();

	bool bIsWinAPIClassRegistered = false;

	virtual IWindow* getFocusedWindow() const override;
};

LRESULT CALLBACK MeteorSpecifiedWindowProcedure(HWND, UINT, WPARAM, LPARAM);

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);