/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Parsing/Module.h>
#include <vector>

struct String;

struct LocatingInterface
{
	static bool FindAllReferences(const String sourceDirectory);

	inline static std::vector<IModule*> modules;
};

