/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WindowManager.h"
#include <Platform/WindowManager/Window.h>
#include <Renderer/Registry.h>
//#include <Core/Application.h>



static int lastSearchIndex = -1;
static String lastSearch = "";

IWindowManager::IWindowManager()
{
	renderContext = nullptr;
	inputManager = nullptr;

}

void IWindowManager::Init()
{
	
}

void IWindowManager::Shutdown()
{
	VulkanRegistry::Shutdown();
}

IWindow* IWindowManager::SearchFor(const String ID)
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

IWindow* IWindowManager::GetFirstWindow()
{
	if (activeWindows.size() > 0)
		return activeWindows[0];

	return nullptr;
}

void IWindowManager::ShowWindow(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->ShowWindow();
		return;
	}

	const size_t activeCount = activeWindows.size();

	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			Temp->ShowWindow();
			return;
		}
	}

}

void IWindowManager::HideWindow(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->HideWindow();
		return;
	}

	const size_t activeCount = activeWindows.size();

	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			Temp->HideWindow();

			return;
		}
	}
}

bool IWindowManager::DrawAttention(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->DrawAttention();
		return true;
	}

	const size_t activeCount = activeWindows.size();

	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			Temp->DrawAttention();

			return true;
		}
	}

	return false;
}

bool IWindowManager::DestroyWindow(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->DestroyWindow();
		return true;
	}

	const size_t activeCount = activeWindows.size();

	for (int i = 0; i < activeCount; i++)
	{
		IWindow* Temp = activeWindows[i];

		if (Temp->windowData.windowID == ID)
		{
			Temp->DestroyWindow();

			return true;
		}
	}

	return false;
}

IWindow* IWindowManager::privSearchFor(const String Name)
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