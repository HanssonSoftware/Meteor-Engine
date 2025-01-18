/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

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

	String f("jancsi123");
	std::vector<String> fg;
	// Insert app specific Initialization here.
	//FontAsset A("RobotoCondensed-Bold.ttf");
	for (int i = 0; i < 20; i++)
	{
		fg.push_back(f);
	}

	//File a("hangya fasz.txt", OPENRULE_WRITE | OPENRULE_READ | OPENRULE_DELETE, OVERRIDERULE_JUST_OPEN);
	//a.Read();
	MeshModel newa;
	try
	{

		//newa.Import("fasz");

	}
	catch (...)
	{

	}
		
	//a.Write("bufos kurvasi �ylehfgyuisehfg �uil ehuoyhse�ouigfyhseij�-�awljd893475e�-mxf.d--.4:_:��KOPJKOIJP�$�[]$��&@�");

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
