/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "ApplicationHelpers.h"

#include <Types/Vector.h>
#include <Types/String.h>
#include <Logging/LogMacros.h>
#include <Platform/WindowManager.h>
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
	Application(const ApplicationInitializationInfo& Info);

	Application(const Application& Nah) = delete;

	virtual ~Application();

	virtual void Init();

	virtual void Run();

	virtual void Shutdown();

	static String GetApplicationDirectory();

	static String GetApplicationName();

	static Application* Get();

	ApplicationInitializationInfo GetAppInfo() const { return appInfo; };

	ApplicationState GetAppState() const { return State; };

	void SetAppState(const ApplicationState newState) { State = newState; };

	IWindowManager* GetWindowManager() const { return windowManager; };

	void SetWindowManager(IWindowManager* NewValue) { windowManager = NewValue; };

	LayerManager* GetLayerManager() const { return layerManager; };

	void SetLayerManager(LayerManager* NewValue) { layerManager = NewValue; };

	IRHIRegistry* GetRenderContext() const { return windowManager->GetRenderContext(); };

	IWindow* GetFirstWindow() const { return windowManager ? windowManager->GetFirstWindow() : nullptr; };

	ILogger logger;

private:
	inline void CreateNativeWindow() const;
protected:

	friend class ILogger;

	IWindowManager* windowManager;

	LayerManager* layerManager;

	ApplicationState State = APPLICATIONSTATE_INITIALIZATION;

	ApplicationInitializationInfo appInfo;

	String applicationName = L"Saxon Proprietary Game Engine";
};

using App = Application;

template<typename T>
void instantiateApp(const ApplicationInitializationInfo* appInfo)
{
	static_assert(std::is_base_of<Application, T>::value, TEXT("T must inherit from Application!"));

	T* App = new T(appInfo);
	App->SetApplicationGetter(App);
	App->Init();

	delete App;
}