/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#include <Common/MemoryManager.h>
#include "Application.h"
#include <Types/String.h>

#include <thread>
#include <File/FileManager.h>

void EditorApplication::Frame()
{
	// Insert app tick here.

	Application::Frame();
}

void EditorApplication::Init()
{
	// Insert app specific Initialization here.

	HANDLE b = nullptr;
	FileOpenInfo openInfo = {};
	openInfo.name = "fasz.txt";
	openInfo.openRules = OpenIfExists;
	openInfo.outputFile = b;

	IFileManager::Get().openFile(&openInfo);
	auto size = IFileManager::Get().getSize("MeteorEditor.txt");

	Application::Init();
}
