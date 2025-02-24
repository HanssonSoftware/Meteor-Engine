/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Common/Delegate.h>
#include <Log/LogMacros.h>

LOG_ADDCATEGORY(Layers);

class Layer
{
	friend class LayerManager;
public:
	Layer(const String Name) noexcept;

	virtual ~Layer() {};

	void operator=(const Layer&) = delete;

	const bool operator==(const Layer& Layer) noexcept
	{
		return this->name == Layer.name;
	}

	virtual void Init() = 0;
	
	virtual void Attached() = 0;

	virtual void Removed() = 0;

	virtual void Update() = 0;

	String getName() const noexcept { return name; }

	void setName(const String NewName) noexcept { name = NewName; }

private:
	const void privAttached() noexcept;

	const void privRemoved() noexcept;

	String name;
};

