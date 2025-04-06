/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <GraphicsEngine/Camera.h>
#include <Types/Vector.h>
#include <Types/String.h>
#include <Log/LogMacros.h>

#ifdef _WIN32
#include <Windows/MinWin.h>
#include <Windows/WinWindowManager.h>
#else

#endif // _WIN32

class String;
class IWindow;
class Camera;
class LayerManager;
struct WindowCreateInfo;
class IGraphicsDevice; 
class InputManager;

LOG_ADDCATEGORY(Application);

enum ApplicationFlags
{
	APPFLAG_NO_WINDOW = 1 << 0,
	APPFLAG_HIDE_CONSOLE_DURING_DEBUG = 1 << 1,
	APPFLAG_NO_FULLSCREEN = 1 << 2,
	APPFLAG_DISPLAY_QUICK_INFO_ABOUT_MEMORY_USAGE = 1 << 3,
	APPFLAG_NO_FILE_LOGGING = 1 << 4,
	APPFLAG_ENABLE_VERBOSE_LOGGING = 1 << 5,
	APPFLAG_START_ONLY_FROM_COMMAND_LINE = 1 << 6,
	APPFLAG_ONLY_ONE_INSTANCE_CAN_RUN = 1 << 7
};

struct ApplicationInitializationInfo
{
	String appName;

	WindowCreateInfo windowCreateInfo;

	String arguments;

	int flags = 0;

};

enum ApplicationState
{
	APPLICATIONSTATE_INITIALIZATION,
	APPLICATIONSTATE_RUNNING,
	APPLICATIONSTATE_RESTARTING,
	APPLICATIONSTATE_SHUTDOWN

};

class Application
{
	friend class Logger;

	static Application* Framework;

	WindowManager* windowManager;

	LayerManager* layerManager;

public:
	Application(const ApplicationInitializationInfo& Info);

	Application(const Application& Nah) = delete;

	virtual ~Application();

	virtual void Init();

	virtual void Run();

	virtual void Shutdown();

	String getApplicationDirectory() const { return applicationLocation; };

	/** Gets the application window size, returns the actual window size with border. Who knows, what else? */
	virtual Vector2<uint32> getWindowSize() const;

	String getApplicationName();

	static Application* Get();

	void setApplicationGetter(Application* app) { Framework = app; };

	ApplicationState getAppState() const { return State; };

	void setAppState(const ApplicationState newState) { State = newState; };

	WindowManager* getWindowManager() const { return windowManager; };

	void setWindowManager(WindowManager* NewValue) { windowManager = NewValue; };

	LayerManager* getLayerManager() const { return layerManager; };

	void setLayerManager(LayerManager* NewValue) { layerManager = NewValue; };

	IGraphicsDevice* getRenderContext() const { return windowManager->getRenderContext(); };

	IWindow* getFirstWindow() const { return windowManager ? windowManager->getFirstWindow() : nullptr; };

	Camera nc;
private:
	inline void instantiateWindow() const;

	inline void checkIfAnotherInstanceIsRunning();
protected:
	ApplicationState State = APPLICATIONSTATE_INITIALIZATION;

	ApplicationInitializationInfo appInfo;

	String applicationName = L"Saxon Proprietary Game Engine";

	String applicationLocation;

	String applicationCodeName;

	String configurationsLocation;
};


template<typename T>
void instantiateApp(const ApplicationInitializationInfo* appInfo)
{
	static_assert(std::is_base_of<Application, T>::value, TEXT("T must inherit from Application!"));

	T* App = new T(appInfo);
	App->setApplicationGetter(App);
	App->Init();

	delete App;
}