/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Array.h>
#include <Module/Module.h>
#include <Core/Application.h>



class Project
{
public:
	String projectName;

	bool Finalize(String* output);

	bool Parse(String* input);

	Array<Module>* modulesReference;

	String intermediatePath;
};

