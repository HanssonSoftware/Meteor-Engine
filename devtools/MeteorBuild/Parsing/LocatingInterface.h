/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Parsing/Module.h>
#include <Types/Array.h>

class String;

struct Locator
{
	static bool FindAllReferences(const String& sourceDirectory);

	static void LocateSources(const String& fullDirectoryToAll, Array<String>& returned);

	static void ListDirectory(const String& directory, Array<String>& returned);

	static void SearchExtensionSpecified(const String& directory, const String& extension, Array<String>& returned);
};

