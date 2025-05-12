/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <vector>

/** Represents a configuration entry with a name and value. */
typedef struct ConfigEntry
{
	/** Sets the value of the configuration entry. */
	void setValue(const String description) { value = description; }

	/** Returns the name of the configuration entry. */
	String GetName() const { return name; }

	String getValue() const { return value; }

	/** Sets the name of the configuration entry. */
	void SetName(String newValue) { name = newValue; }

protected:
	/** The name of the configuration entry. */
	String name;

	/** The value of the configuration entry. */
	String value;

} ConfigEntry;

/** Represents a configuration with a collection of configuration entries. */
class Config
{
public:
	/** Allows ConfigCollection to access private members of Config. */
	friend class ConfigCollection;

	/** Constructor to Initialize the configuration with a name. */
	Config(String name);

	/** Searches for a configuration entry by name. */
	ConfigEntry* searchEntry(String name);

	/** Returns the name of the configuration. */
	String GetName() const { return configurationName; }

	/** Sets the name of the configuration. */
	void SetName(String newName) { configurationName = newName; }

private:
	/** Adds a new entry to the configuration. */
	void addEntry(ConfigEntry entry);

	/** Removes a configuration entry by value. */
	bool removeEntry(ConfigEntry entry);

	/** Removes a configuration entry by index. */
	bool removeEntry(uint32 index);

	/** The name of the configuration. */
	String configurationName = "";

	/** A list of configuration entries. */
	std::vector<ConfigEntry> entries;
};
