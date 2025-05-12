/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Common/Delegate.h>
#include <Logging/LogMacros.h>

class OSLayer;

LOG_ADDCATEGORY(Layers);

class Layer
{
	friend class LayerManager;
public:
	static OSLayer* GetSystemLayer();

	Layer(const String Name) noexcept;

	Layer() = delete;

	virtual ~Layer() = default;

	virtual void Init() = 0;
	
	virtual void Attached() = 0;

	virtual void Removed() = 0;

	virtual void Update() = 0;

	String GetName() const noexcept { return name; }

	void SetName(const String NewName) noexcept { name = NewName; }

	void operator=(const Layer&) = delete;

	const bool operator==(const Layer& Layer) noexcept
	{
		return this->name == Layer.name;
	}

private:
	const void privAttached() noexcept;

	const void privRemoved() noexcept;

	String name;
};

