/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#include "Config.h"


class ConfigCollection
{
public:
	static ConfigCollection& Get();

	void save();

	bool openConfiguration(const String* names, uint32 count = 1);

	Config& searchFor(String name);

	virtual ~ConfigCollection();
private:
	std::vector<Config> containedConfigs;
};

