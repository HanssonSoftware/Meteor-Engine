/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Window.h"
#include <Window/WindowManager.h>
#include <Log/Exception.h>
#include <Types/String.h>
#include <Graphics/GraphicsDevice.h>
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

Window::~Window()
{

}

Vector2<uint32> Window::getSize() const
{
	return windowData->size;
}

bool Window::createWindow(const WindowCreateInfo* CreateInfo)
{
	if (!CreateInfo || !owner)
		return false;

	windowData = CreateInfo;

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

	MR_ASSERT(Handle != INVALID_HANDLE_VALUE, TEXT("Failed to create WinAPI window!"));


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
	setTitle(windowData->windowName);
	ShowWindow((HWND)Handle, SW_SHOWDEFAULT);
#endif
}

void Window::setTitle(String newName)
{

#ifdef _WIN32

#ifdef MR_DEBUG
	newName = String::Format(L"%s <%s>", newName.Chr(), owner->getRenderContext()->getRendererSignatature().Chr());
#endif // MR_DEBUG
	SetWindowText((HWND)Handle, newName.Chr());
#endif // _WIN32
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

