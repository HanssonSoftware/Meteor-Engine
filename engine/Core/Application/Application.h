/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Window/WindowManager.h>
#include <Platform/Microsoft/MinWin.h>
#include <Graphics/Camera.h>
#include <Types/Vector.h>
#include <Types/String.h>
#include <Log/LogMacros.h>
#include <ThirdParty/ImGUI/imgui.h>

class String;
class Window;
class Camera;
class LayerManager;
struct WindowCreateInfo;
class IGraphicsDevice; 
class InputManager;

LOG_ADDCATEGORY(Application);

typedef enum ApplicationFlags
{
	APPFLAG_NO_WINDOW = 1 << 0,
	APPFLAG_HIDE_CONSOLE_DURING_DEBUG = 1 << 1,
	APPFLAG_NO_FULLSCREEN = 1 << 2,
	APPFLAG_DISPLAY_QUICK_INFO_ABOUT_MEMORY_USAGE = 1 << 3,
	APPFLAG_NO_FILE_LOGGING = 1 << 4,
	APPFLAG_ENABLE_VERBOSE_LOGGING = 1 << 5

} ApplicationFlags;

typedef struct ApplicationInitializationInfo
{
	String appName = L'\0';

	const WindowCreateInfo* windowCreateInfo = 0;

	String arguments = L'\0';

	int flags = 0;

} ApplicationInitializationInfo;

typedef enum ApplicationState
{
	Initialization,
	Running,
	Restarting,
	Shutdown

} ApplicationState;

class Application
{
	static Application* Framework;

	WindowManager* windowManager;

	LayerManager* layerManager;

public:
	Application(const ApplicationInitializationInfo* Info);

	Application(const Application& Nah) = delete;

	virtual ~Application();

	/** Called every frame. */
	virtual void Frame(float deltaTime);

	virtual void Init();

	virtual void Run();

	virtual void Shutdown();

	virtual void setAppInfo(const ApplicationInitializationInfo* appInfo);

	const ApplicationInitializationInfo* getAppInfo() const { return appInfo; };
	
	/** Gets the application window size, returns the actual window size with border. Who knows, what else? */
	virtual Vector2<uint32> getWindowSize() const;

	String getApplicationName();

	static Application* Get();

	void drawAttention() const;

	void setApplicationGetter(Application* app) { Framework = app; };

	ApplicationState getAppState() const { return State; };

	void setAppState(const ApplicationState newState) { State = newState; };

	WindowManager* getWindowManager() const { return windowManager; };

	void setWindowManager(WindowManager* NewValue) { windowManager = NewValue; };

	LayerManager* getLayerManager() const { return layerManager; };

	void setLayerManager(LayerManager* NewValue) { layerManager = NewValue; };

	IGraphicsDevice* getRenderContext() const { return windowManager->getRenderContext(); };

	Camera nc;
protected:
	ApplicationState State = Initialization;

	const ApplicationInitializationInfo* appInfo;

	Application() = delete; 

	String applicationName = L"Saxon Proprietary Game Engine";

	String applicationLocation;
};

#ifdef _WIN32
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // _WIN32


template<typename T>
void instantiateApp(const ApplicationInitializationInfo* appInfo)
{
	static_assert(std::is_base_of<Application, T>::value, TEXT("T must inherit from Application!"));

	T* App = new T(appInfo);
	App->setApplicationGetter(App);
	App->Init();

	delete App;
}