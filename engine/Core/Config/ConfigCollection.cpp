/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "ConfigCollection.h"
#include <Types/String.h>
#include <sstream>
#include <fstream>
#include <Common/MemoryManager.h>

bool ConfigCollection::openConfiguration(const String* names, uint32 count)
{
	// Begin iterating through the names.
	for (uint32 i = 0; i < count; i++)
	{
		// Grab the next element from the list
		String indexedConfigName = names[i];

		String realName = indexedConfigName + ".ini";

		// Then open it, if not opened skip.
		std::ifstream stream(realName.Chr(), std::ios_base::in);
		if (!stream.is_open())
			continue;

		Config loadedConfiguration(indexedConfigName);

		// Read the file line-by-line
		std::string ReadLine;
		while (std::getline(stream, ReadLine, '\r'))
		{
			const size_t positionOfDelimiter = ReadLine.find(" = ");

			std::string name = ReadLine.substr(0, positionOfDelimiter);
			std::string value = ReadLine.substr(positionOfDelimiter + 3);

			ConfigEntry entry;
			entry.SetName(name);
			entry.setValue(value);

			loadedConfiguration.addEntry(entry);
		}

		containedConfigs.push_back(loadedConfiguration);
		stream.close();
	}

	return true;
}

ConfigCollection::~ConfigCollection()
{
	save();
}

ConfigCollection& ConfigCollection::Get()
{
	static ConfigCollection instance;
	return instance;
}

void ConfigCollection::save()
{	
	// Begin iterating through the names.
	for (Config& indexedConfig : containedConfigs)
	{
		// Then open it, if not opened skip.
		const String realName = indexedConfig.GetName() + ".ini";
		std::wofstream stream(realName.Chr(), std::ios_base::out);
		if (!stream.is_open())
			continue;

		// Write the file line-by-line
		for (ConfigEntry& entries : indexedConfig.entries)
		{
			//const size_t entrySize = swprintf(NULL, 0, L"%s = %s\r", *entries.GetName(), *entries.getValue());

			//wchar_t* buffer = (wchar_t*)mrmalloc((entrySize + 1) * sizeof(wchar_t));

			//swprintf(buffer, entrySize + 1, L"%s = %s\r", *entries.GetName(), *entries.getValue());

			//stream << buffer;
			//mrfree(buffer);

			stream.flush();
		}

		stream.close();
	}
}

Config& ConfigCollection::SearchFor(String name)
{
	for (auto& config : containedConfigs)
	{
		if (config.GetName() == name)
		{
			return config;
		}
	}

	return containedConfigs[0];
}
