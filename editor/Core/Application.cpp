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

EditorApplication::EditorApplication() 
	: Application()
{
	
}

void EditorApplication::Init()
{
	Application::Init();

	int j = 76;

	Array<int> is(4);
	is.Add(&j, 5);

	is.Add(&j);
	is.Remove(5);


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
