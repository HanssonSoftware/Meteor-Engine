/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>

class Guid
{
public:
	Guid();

	~Guid();

	String getID() const
	{
		return id;
	}

private:
	String id;
};

