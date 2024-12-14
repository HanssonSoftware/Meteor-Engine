/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Application.h"
#include <Common/MemoryManager.h>
#include <Types/String.h>

#include <Log/Exception.h>
#include <Common/Delegate.h>
#include <Graphics/MeshModel.h>
#include <Widgets/Common/FontAsset.h>
#include <Graphics/SceneGraph.h>
#include <Async/SlowTask.h>
#include <iostream>
#include <File/File.h>
#include <Common/Array.h>
#include <Application/Commandlet.h>

Commandlet Adksdm("db", "skfl", false);
Commandlet Adksdmh("dbA", "skfl", true);

EditorApplication::EditorApplication(const ApplicationInitializationInfo* aInfo) 
	: Application(aInfo)
{
	
}

void EditorApplication::Frame(float deltaTime)
{
	// Insert app tick here.

	Application::Frame(deltaTime);
}

#if 0
class Test
{
public:
	Test() {};

	void Calc(/*const String& Sample, float par1, int A*/)
	{
		for (unsigned int i = 0; i < 2000; i++)
		{
			std::wcout << "Count " << i << "\n";
		}
	}
};
#endif

void EditorApplication::Init()
{
	Application::Init();

	// Insert app specific Initialization here.

	//FontAsset A("RobotoCondensed-Bold.ttf");


	File a("picsa.txt");
	a.Read();
	MeshModel newa;
	try
	{

		newa.Import("jbl");

	}
	catch (...)
	{

	}



	//Array<Object> A;
		
	//a.Write(L"bufos kurvasi íylehfgyuisehfg éuil ehuoyhseéouigfyhseijá-éawljd893475eá-mxf.d--.4:_:ÁÉKOPJKOIJPÉ$ß[]$£÷&@£");

	//a.Read();
	SceneGraph::Get().addToRoot(&newa);

	Application::Run();
}

void EditorApplication::Run()
{	
	
}

void EditorApplication::Shutdown()
{
	Application::Shutdown();
}
