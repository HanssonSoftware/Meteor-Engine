/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once

#include "Visual.h"
#include <vector>
#include <Logging/LogMacros.h>

LOG_ADDCATEGORY(Viewport);


class Viewport
{
public:
	friend class Visual;

	Viewport() = default;

	Viewport(const Viewport& Other) = delete;

	~Viewport();

	virtual void Update(float deltaTime) noexcept;

	std::vector<Visual> tickablesList;
protected:


private:
};

void addWidgetToList(Visual* widget);