/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Layer.h"
#include <Core/Application.h>
#include <Layers/LayerManager.h>

OSLayer* Layer::GetSystemLayer()
{
	if (const Application* App = Application::Get())
	{
		if (!App->GetLayerManager()) 
			return nullptr;

		return App->GetLayerManager()->GetSystemLayer();
	}

	return nullptr;
}

Layer::Layer(const String Name) noexcept
	: name(Name)
{
}

const void Layer::privAttached() noexcept
{
	Init();

	MR_LOG(LogLayers, Verbose, "%s has been attached to the engine!", name.IsEmpty() ? "Unnamed Layer" : name.Chr());

	Attached();
}

const void Layer::privRemoved() noexcept
{
	MR_LOG(LogLayers, Verbose, "%s has been removed from the engine!", name.IsEmpty() ? "Unnamed Layer" : name.Chr());

	Removed();
}
