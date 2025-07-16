/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

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
#include <Serialisation/FontImporter.h>
#include <Layers/SystemLayer.h>
#include <Platform/PerformanceTimer.h>

EditorApplication::EditorApplication() 
	: Application()
{
	
}

void EditorApplication::Init()
{
	Application::Init();

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

	String fasdfg(123456789ul);

	Array<int> fasz;
	fasz.Resize(4);

	fasz[0] = 412;
	fasz[1] = 756;
	fasz[2] = 76;
	fasz[3] = 65;

	for (uint32 i = 0; i < fasz.GetSize(); i++)
	{
		int& a = fasz[i];

		Get();
	}

	fasz.Reset();

	String fas("abcABC123éáűőúÉÁŰŐÚßçЖапсржПривет你好こんにちはمرحبا🙂💻!@#$%^&*()_+-=[]{};:'\",.<>/?\\|");
	//MR_LOG(LogTemp, Log, "%s", fas.Chr());

 
	
	String v = Application::Get()->GetLayerManager()->GetSystemLayer()->GetMachineVersion();
	Vector3<float> bcd = Application::Get()->GetLayerManager()->GetSystemLayer()->InspectPixel({1920, 1080});

	FontImporter font("Roboto");

	//	
	//a.Write("bufos kurvasi íylehfgyuisehfg éuil ehuoyhseéouigfyhseijá-éawljd893475eá-mxf.d--.4:_:ÁÉKOPJKOIJPÉ$ß[]$Ł÷&@Ł");

	//SceneGraph::Get().addToRoot(&newa);

	Application::Run();
}

void EditorApplication::Run()
{	
	
}

void EditorApplication::Shutdown()
{
	Application::Shutdown();
}
