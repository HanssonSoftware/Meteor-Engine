/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#include "Window.h"
#include <dwmapi.h>
#include <Types/Vector.h>
#include <Application/Application.h>
#include <../Resources/resource.h>

#pragma comment(lib, "dwmapi.lib")

Window::Window(const wchar_t* Name, Vector2<uint32> Size, HWND Parent)
{
	HINSTANCE Inst = GetModuleHandle(NULL);

	WNDCLASS windowClass = {};
	windowClass.lpszClassName = ApplicationClassName;
	windowClass.hInstance = Inst;
	windowClass.hIcon = (HICON)LoadImage(Inst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0);
	windowClass.lpfnWndProc = WndProc;
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	RegisterClass(&windowClass);

	Handle = CreateWindowExW(
		0, 
		ApplicationClassName, 
		Name, 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		(int)Size.x, 
		(int)Size.y, 
		Parent != nullptr ? Parent : NULL, 
		NULL, 
		Inst, 
		NULL
	);

    BOOL bCanIUseDarkWindowTitlebar = 1;
    DwmSetWindowAttribute(Handle, DWMWA_USE_IMMERSIVE_DARK_MODE, &bCanIUseDarkWindowTitlebar, sizeof(bCanIUseDarkWindowTitlebar));

	Application::Get().setWindowHandle(Handle);
	ShowWindow(Handle, SW_SHOWDEFAULT);
}

Window::~Window()
{
	UnregisterClass(ApplicationClassName, GetModuleHandle(NULL));
}
