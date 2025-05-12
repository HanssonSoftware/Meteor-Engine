/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Config.h"
#include <Windows/Windows.h>

Config::Config(String name)
	: configurationName(name)
{

}

ConfigEntry* Config::searchEntry(String name)
{
	for (auto& indexedEntry : entries)
	{
		if (indexedEntry.GetName() == name)
		{
			return &indexedEntry;
		}
	}

	return nullptr;
}

void Config::addEntry(ConfigEntry entry)
{
	entries.push_back(entry);
}

bool Config::removeEntry(ConfigEntry entry)
{
	const size_t capacity = entries.size();
	for (uint32 i = 0; i < capacity; i++)
	{
		if (entries[i].GetName() == entry.GetName())
		{
			entries[i].SetName(TEXT("INVALIDATED"));
			return true;
		}
	}

	return false;
}

bool Config::removeEntry(uint32 index)
{
	const size_t capacity = entries.size();
	if (index > capacity)
		return false;

	entries[index].SetName(TEXT("INVALIDATED"));
	return true;
}
