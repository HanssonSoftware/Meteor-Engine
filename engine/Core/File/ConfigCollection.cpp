/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "ConfigCollection.h"
#include <Types/String.h>
#include <sstream>
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
		std::string readLine;
		while (std::getline(stream, readLine, '\r'))
		{
			const size_t positionOfDelimiter = readLine.find(" = ");

			std::string name = readLine.substr(0, positionOfDelimiter);
			std::string value = readLine.substr(positionOfDelimiter + 3);

			ConfigEntry entry;
			entry.setName(name);
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
		const String realName = indexedConfig.getName() + ".ini";
		std::wofstream stream(realName.Chr(), std::ios_base::out);
		if (!stream.is_open())
			continue;

		// Write the file line-by-line
		for (ConfigEntry& entries : indexedConfig.entries)
		{
			const size_t entrySize = swprintf(NULL, 0, L"%s = %s\r", *entries.getName(), *entries.getValue());

			wchar_t* buffer = (wchar_t*)mrmalloc((entrySize + 1) * sizeof(wchar_t));

			swprintf(buffer, entrySize + 1, L"%s = %s\r", *entries.getName(), *entries.getValue());

			stream << buffer;
			mrfree(buffer);

			stream.flush();
		}

		stream.close();
	}
}

Config& ConfigCollection::searchFor(String name)
{
	for (auto& config : containedConfigs)
	{
		if (config.getName() == name)
		{
			return config;
		}
	}

	return containedConfigs[0];
}
