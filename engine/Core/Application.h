/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include "Logging/Log.h"
#include <WindowManager/WindowManager.h>

#include "Commandlet.h"
#include <cstdint>


class String;
class IWindow;
class Camera;
class LayerManager;
struct WindowCreateInfo;
class IRHIRegistry;
class InputManager;

LOG_ADDCATEGORY(Application);

struct Application
{
	enum class ECurrentApplicationState { NONE, STARTUP, RUNNING, RESTARTING, SHUTDOWN, DEAD };

	struct
	{
		/** Application name, this would be appearing on the created window. */
		String appName;
		String appNameNoSpaces;

		/** Useful for directories. */
		String appCodeName;

		struct
		{
			String name;

			uint32_t x = 0, y = 0;

		} WindowData;

		union
		{
			double memoryReservePercent;

			double memoryReserveInBytes;

		} Memory;

	};

	Application();

	Application(const Application& Nah) = delete;

	virtual ~Application();

	virtual void Init();

	virtual void Run();

	virtual void Shutdown();

	static String GetApplicationDirectory();

	static String GetApplicationName();

	static Application* Get();

	static void RequestExit(int32_t Code);

	int32_t GetRequestExitCode() const { return appFramework->exitCode; };

	IWindowManager* GetWindowManager() const { return windowManager; };

	void SetWindowManager(IWindowManager* NewValue) { windowManager = NewValue; };

	LayerManager* GetLayerManager() const { return layerManager; };

	void SetLayerManager(LayerManager* NewValue) { layerManager = NewValue; };

	ECurrentApplicationState GetAppState() const { return state; };

	void SetAppState(const ECurrentApplicationState& newState) { state = newState; };

protected:

	int32_t exitCode = 0;

	IWindowManager* windowManager;

	LayerManager* layerManager;

	ECurrentApplicationState state = ECurrentApplicationState::NONE;

	static inline Application* appFramework = nullptr;
};

Application* GetApplication();

#define IMPLEMENT_APPLICATION(ApplicationClass) \
	/*extern "C" __declspec(dllexport)*/ int32_t LaunchApplication(int32_t ArgumentCount, char* Arguments[]) \
	{	\
		static ApplicationClass instance; \
        instance.Init(); \
		Commandlet::Initialize(); \
        instance.Run(); \
        instance.Shutdown(); \
        return instance.GetRequestExitCode(); \
	}