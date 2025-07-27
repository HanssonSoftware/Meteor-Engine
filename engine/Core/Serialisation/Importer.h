/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>

class IImporter
{
public:
	IImporter() = default;

	virtual ~IImporter();

	virtual void loadAsset(const String Name) = 0;

	String GetName() const
	{
		return name;
	}

	void SetName(const String NewValue)
	{
		name = NewValue;
	}

private:
	String name;

	void* data;
};

