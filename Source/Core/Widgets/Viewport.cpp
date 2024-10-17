/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

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
		MR_LOG(LogViewport, Error, TEXT("Invalid widget!"));
		return;
	}

	getSubsystem<Viewport>().tickablesList.push_back(*widget);
}
