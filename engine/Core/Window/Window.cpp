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


IWindow::IWindow(WindowManager* newOwner)
	: owner(newOwner)
{

}

IWindow::~IWindow()
{

}

Vector2<uint32> IWindow::getSize() const
{
	return windowData.size;
}
#if 0
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

#endif