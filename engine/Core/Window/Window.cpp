/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Window.h"
#include <Log/Exception.h>
#include <Window/WindowManager.h>
#include <GraphicsEngine/GraphicsDevice.h>
#ifdef _WIN32
#include <dwmapi.h>
//#include <../Resources/resource.h>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "UxTheme.lib")
#endif // _WIN32
#include <mutex>

LOG_ADDCATEGORY(Window);


Window::Window(WindowManager* newOwner)
	: owner(newOwner)
{

}

#ifdef _WIN32
Window::Window(const HWND wnd)
{
	if (!wnd) return;

	wchar_t buffer[256];
	const int length = GetWindowTextW(wnd, buffer, 256);
}
#endif

Window::~Window()
{

}

Vector2<uint32> Window::getSize() const
{
	return windowData.size;
}

bool Window::createWindow(const WindowCreateInfo* CreateInfo)
{
	if (!CreateInfo || !owner)
		return false;

	windowData = *CreateInfo;

#ifdef _WIN32
	HINSTANCE Inst = GetModuleHandle(NULL);

	RECT windowRect = { 0, 0, (LONG)CreateInfo->size.x, (LONG)CreateInfo->size.y };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, 0);

	Handle = CreateWindowEx(
		/*WS_EX_ACCEPTFILES*/0,
		ApplicationClassName,
		CreateInfo->windowName.Chr(),
		CreateInfo->flags == -1 ? WS_OVERLAPPEDWINDOW : CreateInfo->flags,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		Inst,
		NULL
	);

	if (Handle == INVALID_HANDLE_VALUE)
	{
		THROW_EXCEPTION("Failed to Create WinAPI Window!");
		return false;
	}


	static const constexpr BOOL bCanIUseDarkWindowTitlebar = 1;
	DwmSetWindowAttribute((HWND)Handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &bCanIUseDarkWindowTitlebar, sizeof(bCanIUseDarkWindowTitlebar));
#endif
	//SetWindowTheme((HWND)Handle, L"Explorer", 0);
	return true;
}

void Window::destroyWindow()
{
#ifdef _WIN32
	DestroyWindow((HWND)Handle);
#endif // _WIN32

	Handle = nullptr;
}

void Window::showWindow()
{
	if (!Handle)
		return;

#ifdef _WIN32
	setTitle(windowData.windowName);
	ShowWindow((HWND)Handle, SW_SHOWDEFAULT);
#endif
}

void Window::setTitle(const String newName)
{
	if (!Handle)
		return;

	String name = newName;
#ifdef _WIN32

#ifdef MR_DEBUG
	name = String::Format(L"%s - %s", newName.Chr(), owner->getRenderContext()->getRendererSignatature().Chr());
#endif // MR_DEBUG
	SetWindowText((HWND)Handle, name.Chr());
#endif // _WIN32
}

void Window::hideWindow()
{
	if (!Handle)
		return;

#ifdef _WIN32
	ShowWindow((HWND)Handle, SW_HIDE);
#endif
}

bool Window::drawAttention()
{
	if (!Handle)
		return false;

#ifdef _WIN32
	FLASHWINFO flashInfo = {};
	flashInfo.cbSize = sizeof(flashInfo);
	flashInfo.hwnd = (HWND)Handle;
	flashInfo.dwFlags = FLASHW_TRAY;
	flashInfo.uCount = 0;
	flashInfo.dwTimeout = 0;

	return FlashWindowEx(&flashInfo);
#else
	THROW_EXCEPTION("Not Implemented!");
#endif // _WIN32
}

constexpr const int Window::evaluateFlags(int Flags) noexcept
{
	return 0;
}

void* Window::getWindowHandle()
{
	if (!Handle)
		return nullptr;

#ifdef _WIN32
	return (HWND)Handle;
#else
	THROW_EXCEPTION("Not Implemented!");
#endif // _WIN32

	return nullptr;
}

