/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <vector>

//typedef struct ModuleProperty
//{
//	ModuleProperty(const String path, const String name)
//		: path(path)
//		, name(name)
//	{
//
//	};
//
//	String path;
//
//	String name;
//
//} ModuleProperty;

class DirectoryExplorer
{
public:
	DirectoryExplorer();

	~DirectoryExplorer();

	inline void goBack(String& input);

	void startExpedition(String directory);

	std::vector<String> modules;
};

