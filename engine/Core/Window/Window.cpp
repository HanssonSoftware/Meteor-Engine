/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Window.h"
#include <Log/Exception.h>
#include "WindowManager.h"
#include <Graphics/GraphicsDevice.h>

#ifdef _WIN32
#include <dwmapi.h>
//#include <../Resources/resource.h>
#pragma comment(lib, "dwmapi.lib")
#endif // _WIN32
#include <mutex>


Window::Window()
{

}

Window::~Window()
{

}

Vector2<uint32> Window::getSize() const
{
	return windowData->size;
}

bool Window::createWindow(const WindowCreateInfo* CreateInfo)
{
	if (!CreateInfo)
		return false;

	windowData = CreateInfo;

#ifdef _WIN32
	HINSTANCE Inst = GetModuleHandle(NULL);

	Handle = CreateWindowEx(
		0,
		ApplicationClassName,
		CreateInfo->windowName.Chr(),
		CreateInfo->flags == -1 ? WS_OVERLAPPEDWINDOW : CreateInfo->flags,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(int)CreateInfo->size.x,
		(int)CreateInfo->size.y,
		NULL,
		NULL,
		Inst,
		NULL
	);

	if (Handle == INVALID_HANDLE_VALUE)
	{
		THROW_EXCEPTION("Failed to create WinAPI window!");
	}

	static const constexpr BOOL bCanIUseDarkWindowTitlebar = 1;
	DwmSetWindowAttribute((HWND)Handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &bCanIUseDarkWindowTitlebar, sizeof(bCanIUseDarkWindowTitlebar));
#endif

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
#ifdef _WIN32
	ShowWindow((HWND)Handle, SW_SHOWDEFAULT);
#endif
}

void Window::hideWindow()
{
#ifdef _WIN32
	ShowWindow((HWND)Handle, SW_HIDE);
#endif
}

constexpr const int Window::evaluateFlags(int Flags) noexcept
{
	return 0;
}

void* Window::getWindowHandle()
{
#ifdef _WIN32
	return (HWND)Handle;
#endif // _WIN32

	return nullptr;
}

