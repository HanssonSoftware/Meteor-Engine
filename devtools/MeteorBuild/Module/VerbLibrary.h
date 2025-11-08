/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Delegate.h>

class VerbLibrary
{
public:
	static VerbLibrary& Get()
	{
		static VerbLibrary instance;
		return instance;
	}
	
	void RegisterVerb() {};
};

