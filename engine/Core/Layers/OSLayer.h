/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
//#include <Common/Delegate.h>
#include <Log/LogMacros.h>
#include <Types/Vector.h>

LOG_ADDCATEGORY(SystemLayer);

class OSLayer
{
	friend class LayerManager;

public:

	virtual Vector3<float> inspectPixel(const Vector2<float> screenCoordinates) = 0;

	virtual String getMachineName() const = 0;
};
