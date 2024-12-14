/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/Microsoft/MinWin.h>
//#include <Window/WindowManager.h>
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
class WindowManager;
class InputManager;

LOG_ADDCATEGORY(Application);


typedef struct ApplicationInitializationInfo
{
	String appName;

	const WindowCreateInfo* windowCreateInfo;

	String arguments;

} ApplicationInitializationInfo;

typedef enum EApplicationState
{
	Initialization,
	Running,
	Restarting,
	Shutdown

} EApplicationState;

class Application
{
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

	/** Sets the application name, maybe changes the title text in the future. */
	virtual void setApplicationName(const String Name);

	/** Gets the application name usually, returns what's in the title. Who knows? */
	virtual String getApplicationName();
	
	/** Gets the application window size, returns the actual window size with border. Who knows, what else? */
	virtual Vector2<uint32> getWindowSize() const;

	static Application* Get();

	void drawAttention() const;

	String getExecutableDir() const;

	void setApplicationGetter(Application* app) { Framework = app; };

	EApplicationState getAppState() const { return State; };

	void setAppState(const EApplicationState newState) { State = newState; };

	WindowManager* getWindowManager() const { return windowManager; };

	void setWindowManager(WindowManager* NewValue) { windowManager = NewValue; };

	LayerManager* getLayerManager() const { return layerManager; };

	void setLayerManager(LayerManager* NewValue) { layerManager = NewValue; };

	Camera nc;
protected:
	EApplicationState State = Initialization;

	const ApplicationInitializationInfo* appInfo;

	Application() = delete; 

	String applicationName = L"Saxon Proprietary Game Engine";

private:
	static Application* Framework;

	WindowManager* windowManager;

	LayerManager* layerManager;
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