/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Types/Vector.h>

static constexpr const wchar_t* ApplicationClassName = L"MeteorApplication";

class Window
{
public:
	friend class Application;

	Window(const wchar_t* Name, Vector2<uint32> Size, HWND Parent);

	~Window();

	HWND getWindowHandle() const { return Handle; };
private:
	HWND Handle = nullptr;
};

