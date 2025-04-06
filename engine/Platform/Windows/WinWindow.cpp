/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WinWindow.h"
#include <Windows/MinWin.h>
#include <Log/LogMacros.h>

void WinWindow::destroyWindow()
{

}

void WinWindow::showWindow()
{
}

void WinWindow::setTitle(const String newName)
{
}

void WinWindow::hideWindow()
{
}

bool WinWindow::drawAttention()
{
	UNIMPLEMENTED("fasdfawefr");
	return false;
}

void* WinWindow::getWindowHandle()
{
	return (HWND)Handle;
}
