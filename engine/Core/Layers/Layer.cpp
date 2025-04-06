/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Layer.h"


Layer::Layer(const String Name) noexcept
	: name(Name)
{
	
}

const void Layer::privAttached() noexcept
{
	MR_LOG(LogLayers, Verbose, TEXT("Layer %s Has Been Attached to the Engine!"), name.isEmpty() ? L"Unnamed Layer" : name.Chr());

	Attached();
}

const void Layer::privRemoved() noexcept
{
	MR_LOG(LogLayers, Verbose, TEXT("Layer %s Has Been Removed from the Engine!"), name.isEmpty() ? L"Unnamed Layer" : name.Chr());

	Removed();
}
