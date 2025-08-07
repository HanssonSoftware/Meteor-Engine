/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "WindowManager.h"
#include <WindowManager/Window.h>
//#include <Core/Application.h>


static int lastSearchIndex = -1;
static String lastSearch = "";

IWindowManager::IWindowManager()
{
	inputManager = nullptr;
}

void IWindowManager::Init()
{
	
}

void IWindowManager::Shutdown()
{
}

IWindow* IWindowManager::SearchFor(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.GetSize() && lastSearch == ID)
	{
		return activeWindows[lastSearchIndex];
	}

	const size_t activeCount = activeWindows.GetSize();
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
	if (activeWindows.GetSize() > 0)
		return activeWindows[0];

	return nullptr;
}

void IWindowManager::ShowWindow(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.GetSize() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->ShowWindow();
		return;
	}

	const size_t activeCount = activeWindows.GetSize();

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
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.GetSize() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->HideWindow();
		return;
	}

	const size_t activeCount = activeWindows.GetSize();

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
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.GetSize() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->DrawAttention();
		return true;
	}

	const size_t activeCount = activeWindows.GetSize();

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
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.GetSize() && lastSearch == ID)
	{
		activeWindows[lastSearchIndex]->DestroyWindow();
		return true;
	}

	const size_t activeCount = activeWindows.GetSize();

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
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.GetSize() && lastSearch == Name)
	{
		return activeWindows[lastSearchIndex];
	}

	const size_t activeCount = activeWindows.GetSize();
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