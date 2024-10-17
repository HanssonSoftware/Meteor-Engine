/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#include <Application/Subsystem.h>
#include "Visual.h"
#include <vector>
#include <Log/LogMacros.h>

LOG_ADDCATEGORY(Viewport);


class Viewport : public Subsystem
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