/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>

class Layer
{
public:
	Layer(const String Name) noexcept;
	
	Layer(const Layer&) = delete;

	Layer() = delete;

	void operator=(const Layer&) = delete;

	const bool operator==(const Layer& Layer) noexcept
	{
		return this->name == Layer.name;
	}

	virtual void Init() = 0;
	
	virtual void eventAttached() = 0;

	virtual void Update() = 0;

	String getName() const noexcept { return name; }

	void setName(const String NewName) noexcept { name = NewName; }
private:
	String name;
};

