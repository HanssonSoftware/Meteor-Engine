/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Application.h"
#include <MemoryManager.h>
#include <Types/String.h>
#include <Layers/LayerManager.h>
#include <iostream>
#include <Platform/File.h>
#include <Types/Array.h>
#include <Commandlet.h>
#include <Platform/FileManager.h>
#include "EditorLayer.h"
#include <Layers/SystemLayer.h>
#include <Platform/PerformanceTimer.h>
#include <Types/Pointers.h>
#include <Platform/Paths.h>
#include <Platform/Platform.h>

EditorApplication::EditorApplication() 
	: Application()
{
	appName = "Meteor Editor";
	appNameNoSpaces = "MeteorEditor";
	appCodeName = "Astronaut";

	WindowData.x = 1280;
	WindowData.y = 720;

	Memory.memoryReservePercent = 0.15;
}

void EditorApplication::Init()
{
	Application::Init();
	
	Paths::GetSystemDirectory();
	Paths::GetDocumentsDirectory();

	int32_t j = 76;

	Array<int32_t> is(4);
	for (uint32_t i = 0; i < is.GetSize(); i++)
	{
		int32_t& a = is[i];

		a = i + 1000;
	}

	is.Add(j);
	is.Pop(3);

	for (int32_t i : is)
	{
		int32_t jsa = 523;
	}

	//MemoryManager::Deallocate(asd);

	EditorLayer Super("Editor Layer");
	Application::Get()->GetLayerManager()->PushLayer(&Super);

	if (Platform::IsRunningAnAnotherInstance())
	{
		MessageBoxDescriptor info = {};
		info.Title = "Multiple Instances found!";
		info.Description = "";
		info.Type = MESSAGEBOXTYPE_INFORMATION | MESSAGEBOXBUTTONS_OK;

		Layer::GetSystemLayer()->AddMessageBox(&info);
		
		Shutdown();
	}

	Application::Run();
}

void EditorApplication::Run()
{	
	
}

void EditorApplication::Shutdown()
{
	Application::Shutdown();
}
