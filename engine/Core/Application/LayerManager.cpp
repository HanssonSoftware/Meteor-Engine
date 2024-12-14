/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "LayerManager.h"
#include "Layer.h"

void LayerManager::addLayer(Layer* Instance)
{
	if (!Instance)
		return;

	Instance->eventAttached();
	layers.push_back(Instance);
}

void LayerManager::removeLayer(const Layer* Instance)
{
	const size_t size = layers.size();
	for (size_t i = 0; i < size; i++)
	{
		if (layers[i] == Instance)
		{
			layers[i] = nullptr;
		}
	}
}

void LayerManager::removeLayer(const String Name)
{
	const size_t size = layers.size();
	for (size_t i = 0; i < size; i++)
	{
		if (layers[i]->getName() == Name)
		{
			layers[i] = nullptr;
		}
	}
}

void LayerManager::updateLayer()
{
	for (Layer*& indexed : layers)
	{
		indexed->Update();
	}
}
