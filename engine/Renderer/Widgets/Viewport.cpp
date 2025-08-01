/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Viewport.h"

Viewport::~Viewport()
{
	tickablesList.clear();
}

void Viewport::Update(float deltaTime) noexcept
{
	for (Visual& indexedVisual : tickablesList)
	{
		indexedVisual.Update(deltaTime);
	}
}

void addWidgetToList(Visual* widget)
{
	if (!widget) {
		MR_LOG(LogViewport, Error, "Invalid widget!");
		return;
	}

	//tickablesList.push_back(*widget);
}
