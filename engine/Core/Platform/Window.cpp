/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Window.h"
#include <Platform/WindowManager.h>


IWindow::IWindow(IWindowManager* newOwner)
	: owner(newOwner)
{

}