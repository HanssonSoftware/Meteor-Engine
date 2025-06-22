/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
class IModule;

struct ScriptParser
{
	static void BeginParse(const IModule& module);

	static const char* ParseIndex(const char* pos);

	static const char* ParseBlock(const char* pos);
};

