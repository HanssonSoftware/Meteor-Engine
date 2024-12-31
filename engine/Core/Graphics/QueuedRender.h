/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Common/Delegate.h>

class IQueuedRender
{
public:
	virtual void Render();

	virtual void bindRender() = 0;

	virtual void unbindRender() = 0;

	bool getIsVisible() const { return bIsVisible; };
	
	void setIsVisible(bool newValue) { bIsVisible = newValue; visibilityChanged.invoke(newValue); };

	Delegate<bool> visibilityChanged;
private:
	bool bIsVisible = true;
};

