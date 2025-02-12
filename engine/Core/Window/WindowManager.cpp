/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WindowManager.h"
#include <Window/Window.h>
#include <Application/Application.h>
#include <GraphicsEngine/D3D11/Direct3DDevice.h>
//#include <Graphics/Vulkan/VulkanDevice.h>
#ifdef _WIN32
#include <../Resources/resource.h>
#endif // _WIN32
#include <Log/Exception.h>

static int lastSearchIndex = -1;
static String lastSearch = "";

WindowManager::WindowManager()
{
	renderContext = nullptr;
	inputManager = nullptr;

#ifdef _WIN32

#endif // _WIN32
}

WindowManager::~WindowManager()
{
	if (renderContext)
	{
		renderContext->cleanUp();
		delete renderContext;
		renderContext = nullptr;
	}

#ifdef _WIN32
	UnregisterClass(ApplicationClassName, GetModuleHandle(NULL));
#endif // _WIN32
}

WindowManager& WindowManager::Get()
{
	static WindowManager instance;
	return instance;
}

Window* WindowManager::createWindow(const WindowCreateInfo* CreateInfo)
{
	if (CreateInfo->windowID.isEmpty())
	{
		MR_LOG(LogWindowManager, Error, TEXT("CreateInfo->windowID is Empty!"));
		return nullptr;
	}

	if (!bIsWinAPIClassRegistered)
	{
		if (!registerWindowClass())
		{
			THROW_EXCEPTION("Failed to Register Window Class!");
		}
	}

	Window* newWindow = new Window(this);
	if (newWindow->createWindow(CreateInfo))
	{
		activeWindows.push_back(newWindow);

		if (getFirstWindow() == newWindow)
		{
			renderContext = new IDirect3DDevice();
			renderContext->Init();
		}

		return newWindow;
	}
	else
	{
		THROW_EXCEPTION("Failed to Create Window!");
	}

	return nullptr;
}

Window* WindowManager::searchFor(const String ID)
{
	if (lastSearchIndex != -1 && lastSearchIndex < activeWindows.size() && lastSearch == ID)
	{
		return activeWindows[lastSearchIndex];
	}

	const size_t activeCount = activeWindows.size();
	for (int i = 0; i < activeCount; i++)
	{
		Window* Temp = activeWindows[i];

		if (Temp->windowData->windowID == ID)
		{
			lastSearchIndex = i;
			lastSearch = ID;
			return Temp;
		}
	}

	return nullptr;
}

Window* WindowManager::getFirstWindow()
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
		Window* Temp = activeWindows[i];

		if (Temp->windowData->windowID == ID)
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
		Window* Temp = activeWindows[i];

		if (Temp->windowData->windowID == ID)
		{
			Temp->hideWindow();

			return;
		}
	}
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
		Window* Temp = activeWindows[i];

		if (Temp->windowData->windowID == ID)
		{
			Temp->destroyWindow();

			return true;
		}
	}

	return false;
}

#ifdef _WIN32
inline bool WindowManager::registerWindowClass()
{
	HINSTANCE instance = GetModuleHandle(NULL);
	HICON ico = (HICON)LoadImage(instance, MAKEINTRESOURCE(IDI_DEFAULTAPPICON), IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR);

	WNDCLASSEX windowClass = {};
	windowClass.lpszClassName = ApplicationClassName;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hInstance = instance;
	windowClass.hIcon = ico;
	windowClass.hIconSm = ico;
	windowClass.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&windowClass))
	{
		THROW_EXCEPTION("Failed to register WinAPI class!");
		return false;
	}

	bIsWinAPIClassRegistered = true;
	return true;
}
#endif
