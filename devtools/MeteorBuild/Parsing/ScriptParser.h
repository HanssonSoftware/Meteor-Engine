/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
class Module;

struct ScriptParser
{
	static void BeginParse(const Module& module);

	static const char* ParseIndex(const char* pos);

	static const char* ParseBlock(const char* pos);
};

