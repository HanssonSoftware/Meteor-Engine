/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Intermediate.h"

#include <FileManager.h>

bool Intermediate::SearchIntermediateFiles(String intermediateDirectory)
{
	if (!intermediateDirectory.IsEmpty())
	{
		std::vector<String> dirs;
		FileManager::ListDirectory(intermediateDirectory, dirs);


		return dirs.size() > 0 ? true : false;
	}

	return false;
}
