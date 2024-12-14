/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Application.h"
#include <string.h>
#include <Core/Application/Commandlet.h>

int main(int argc, char* argv[])
{
	ICommandlet::Get().Parse(argc, argv);

	for (int i = 0; i < argc; i++)
	{
		char* k = argv[i];
		auto u = strlen(argv[i]);
	}

	return 6;
}
//BuildSystemApplication::BuildSystemApplication(const ApplicationInitializationInfo* Info)
//	: Application(Info)
//{
//}
//
//void BuildSystemApplication::Frame(float deltaTime)
//{
//}
//
//void BuildSystemApplication::Init()
//{
//	Application::Init();
//}
//
//void BuildSystemApplication::Run()
//{
//}
//
//void BuildSystemApplication::Shutdown()
//{
//}
