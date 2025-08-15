/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Application.h"
#include <MemoryManager.h>
#include <Types/String.h>
#include <Common/Delegate.h>
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

EditorApplication::EditorApplication() 
	: Application()
{
	appName = "Meteor Editor";
	appCodeName = "Astronaut";

	Memory.memoryReservePercent = 0.15;
}

void EditorApplication::Init()
{
	Application::Init();
	
	Paths::GetSystemDirectory();
	Paths::GetDocumentsDirectory();

	int j = 76;

	Array<int> is(4);
	for (uint32 i = 0; i < is.GetSize(); i++)
	{
		int& a = is[i];

		a = i + 1000;
	}

	is.Add(j);
	is.Pop(3);

	for (int i : is)
	{
		int jsa = 523;
	}

	//MemoryManager::Deallocate(asd);

	EditorLayer Super("geci");
	Application::Get()->GetLayerManager()->PushLayer(&Super);

	if (Layer::GetSystemLayer()->IsRunningAnAnotherInstance())
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
