/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <type_traits>

class Module
{
public:
	virtual void StartupModule() = 0;

	virtual void ShutdownModule() = 0;

	String GetName() const { return name; }
protected:
	String name;
};

#define IMPLEMENT_MODULE(ModuleClass)																		 \
	static_assert(std::is_base_of<Module, ModuleClass>::value, "ModuleClass does not inherit from Module!"); \
	extern "C" __declspec(dllexport) Module* InitialiseModule()												 \
	{																										 \
		return new ModuleClass();																			 \
	}
