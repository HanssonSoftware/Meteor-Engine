/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "WindowsWindow.h"
#include <Logging/LogMacros.h>
#include <Windows/WindowsWindowManager.h>
#include <Layers/SystemLayer.h>

//#include <../Resources/resource.h>
#include <dwmapi.h>
#include <winuser.h>
#include <Core/Application.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "UxTheme.lib")

#ifdef MR_DEBUG
static constexpr bool bIsDebug = true;
#else
static constexpr bool bIsDebug = false;
#endif // MR_DEBUG


WindowsWindow::WindowsWindow(WindowsWindowManager* owner) 
	: IWindow(owner)
{

}

void WindowsWindow::DestroyWindow()
{
	if (handle == nullptr)
	{
		MR_LOG(LogWindowManager, Error, "Unable to destroy window %s", windowData.windowID.Chr());
		return;
	}

	if (::DestroyWindow((HWND)handle) == 0)
	{
		MR_LOG(LogWindowManager, Warn, "Window does not destroyed fully: %s", Layer::GetSystemLayer()->GetError().Chr());
 		return;
	}

	handle = nullptr;
}

void WindowsWindow::ShowWindow()
{
	if (handle == nullptr)
	{
		MR_LOG(LogWindowManager, Error, "Invalid window handle!");
		return;
	}

	::ShowWindow((HWND)handle, /*SW_SHOWDEFAULT*/ 10);
}

void WindowsWindow::SetTitle(const String& newName)
{
	if (handle == nullptr)
	{
		MR_LOG(LogWindowManager, Error, "Invalid window handle!");
		return;
	}

	windowData.windowName = newName;

	const wchar_t* buffer = Layer::GetSystemLayer()->ConvertToWide(windowData.windowName.Chr());
	
	if (::SetWindowTextW((HWND)handle, buffer) == 0)
	{
		MR_LOG(LogWindowManager, Error, "Unable to set window title: %s", Layer::GetSystemLayer()->GetError().Chr());
		delete[] buffer;
		return;
	}

	delete[] buffer;
}

void WindowsWindow::HideWindow()
{
	if (handle == nullptr)
	{
		MR_LOG(LogWindowManager, Error, "Invalid window handle!");
		return;
	}

	::ShowWindow((HWND)handle, /*SW_HIDE*/ 0);
}

void WindowsWindow::DrawAttention()
{
	if (handle == nullptr)
	{
		MR_LOG(LogWindowManager, Error, "Invalid window handle!");
		return;
	}

	FLASHWINFO flashInfo = {};
	flashInfo.cbSize = sizeof(flashInfo);
	flashInfo.hwnd = (HWND)handle;
	flashInfo.dwFlags = FLASHW_TRAY;
	flashInfo.uCount = 0;
	flashInfo.dwTimeout = 0;

	FlashWindowEx(&flashInfo);
}

void* WindowsWindow::GetWindowHandle()
{
	return (HWND)handle;
}
