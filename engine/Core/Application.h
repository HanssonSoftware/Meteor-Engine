/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "ApplicationHelpers.h"

#include <Types/Vector.h>
#include <Types/String.h>
#include <Logging/LogMacros.h>
#include <Platform/WindowManager/WindowManager.h>
//#include <Windows/Windows.h>

#ifdef _WIN64
#include <Windows/WindowsWindowManager.h>
#else

#endif // _WIN64

struct String;
class IWindow;
class Camera;
class LayerManager;
struct WindowCreateInfo;
class IRHIRegistry;
class InputManager;

LOG_ADDCATEGORY(Application);

struct Application
{
	Application();

	Application(const Application& Nah) = delete;

	virtual ~Application();

	virtual void Init();

	virtual void Run();

	virtual void Shutdown();

	static bool InstantiateApplication(Application* newInstance, const ApplicationInitializationInfo* appCreateInfo);

	static String GetApplicationDirectory();

	static String GetApplicationName();

	static Application* Get();

	static void RequestExit(int Code);

	static const int GetRequestExitCode();

	ApplicationInitializationInfo GetAppInfo() const { return appInfo; };

	ApplicationState GetAppState() const { return State; };

	void SetAppState(const ApplicationState newState) { State = newState; };

	IWindowManager* GetWindowManager() const { return windowManager; };

	void SetWindowManager(IWindowManager* NewValue) { windowManager = NewValue; };

	LayerManager* GetLayerManager() const { return layerManager; };

	void SetLayerManager(LayerManager* NewValue) { layerManager = NewValue; };

	IRHIRegistry* GetRenderContext() const { return windowManager->GetRenderContext(); };

	IWindow* GetFirstWindow() const { return windowManager ? windowManager->GetFirstWindow() : nullptr; };

private:
	inline void CreateNativeWindow() const;

protected:
	int exitCode = 0;

	IWindowManager* windowManager;

	LayerManager* layerManager;

	ApplicationState State = APPLICATIONSTATE_INITIALIZATION;

	ApplicationInitializationInfo appInfo;

	String applicationName = L"Saxon Proprietary Game Engine";
};

using App = Application;