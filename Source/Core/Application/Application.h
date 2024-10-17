/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Types/Vector.h>
#include <Types/String.h>
#include <Log/LogMacros.h>

class IGraphicsDevice;

LOG_ADDCATEGORY(Application);

typedef struct
{
	const wchar_t* windowName;

	Vector2<uint32> size;

} WindowCreateInfo;

typedef struct
{
	const wchar_t* appName;

	const WindowCreateInfo* windowCreateInfo;

	int* argumentCount;

	char* arguments;

} ApplicationInitializationInfo;

class Application
{
public:
	const ApplicationInitializationInfo* appInfo;

	Application(const String Name, Vector2<uint32> Size);

	Application(const Application& Nah) = delete;

	/** Called every frame. */
	virtual void Frame();

	virtual void Init();

	virtual void setAppInfo(const ApplicationInitializationInfo* appInfo);

	virtual ~Application();

	virtual void setWindowHandle(void* NewHandle) { windowHandle = NewHandle; }

	virtual void* getWindowHandle();

	/** Sets the application name, maybe changes the title text in the future. */
	virtual void setApplicationName(const String Name);

	/** Gets the application name usually, returns what's in the title. Who knows? */
	virtual String getApplicationName() const { return applicationName; }	
	
	/** Sets the application window size. */
	virtual void setWindowSize(Vector2<uint32> NewSize) { windowSize = NewSize; }

	/** Gets the application window size, returns the actual window size with border. Who knows, what else? */
	virtual Vector2<uint32> getWindowSize() const { return windowSize; }

	static Application& Get() { return *Framework; };

	void drawAttention();

	String getExecutableDir() const;

	bool bIsAppRunning = false;

	void setApplicationGetter(Application* app) { Framework = app; };
protected:

	Application() = default; 

	String applicationName = L"Saxon Proprietary Game Engine";

	void* windowHandle;

	Vector2<uint32> windowSize;

private:
	static Application* Framework;
};

#ifdef _WIN32
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#endif // _WIN32


template<typename T>
void instantiateApp(const ApplicationInitializationInfo* appInfo)
{
	static_assert(std::is_base_of<Application, T>::value, TEXT("T must inherit from Application!"));

	Application* App = new T();
	App->setApplicationGetter(App);
	App->setAppInfo(appInfo);
	App->Init();

	delete App;
}