/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WindowManager.h"
#include <Window/Window.h>
//#include <Application/Application.h>
#include <GraphicsEngine/D3D11/Direct3DDevice.h>
#include <Log/Exception.h>



static int lastSearchIndex = -1;
static String lastSearch = "";

WindowManager::WindowManager()
{
	renderContext = nullptr;
	inputManager = nullptr;

	Init();
}

WindowManager::~WindowManager()
{
}

void WindowManager::Init()
{
}

void WindowManager::Destroy()
{
	if (renderContext)
	{
		renderContext->cleanUp();

		delete renderContext;
		renderContext = nullptr;
	}
}

IWindow* WindowManager::searchFor(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		return activeWindows[lastSearchIndex];
	}

	const size_t activeCount = activeWindows.size();
	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			lastSearchIndex = i;
			lastSearch = ID;
			return Temp;
		}
	}

	return nullptr;
}

IWindow* WindowManager::getFirstWindow()
{
	if (activeWindows.size() > 0)
		return activeWindows[0];

	return nullptr;
}

void WindowManager::showWindow(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->showWindow();
		return;
	}

	const size_t activeCount = activeWindows.size();

	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			Temp->showWindow();

			return;
		}
	}

}

void WindowManager::hideWindow(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->hideWindow();
		return;
	}

	const size_t activeCount = activeWindows.size();

	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			Temp->hideWindow();

			return;
		}
	}
}

bool WindowManager::drawAttention(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->drawAttention();
		return true;
	}

	const size_t activeCount = activeWindows.size();

	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			Temp->drawAttention();

			return true;
		}
	}

	return false;
}

bool WindowManager::destroyWindow(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->destroyWindow();
		return true;
	}

	const size_t activeCount = activeWindows.size();

	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			Temp->destroyWindow();

			return true;
		}
	}

	return false;
}

IWindow* WindowManager::privSearchFor(const String Name)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == Name)
	{
		return activeWindows[lastSearchIndex];
	}

	const size_t activeCount = activeWindows.size();
	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowName == Name)
		{
			lastSearchIndex = i;
			lastSearch = Name;
			return Temp;
		}
	}

	return nullptr;
}