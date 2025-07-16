/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "LayerManager.h"
#include "Layer.h"
#include "SystemLayer.h"
#include <Windows/WindowsLayer.h>

LayerManager::LayerManager()
{

}

void LayerManager::Init()
{
	PushSystemLayer();
}

void LayerManager::Shutdown()
{
	for (Layer*& indexed : layers)
	{
		indexed->privRemoved();
		indexed = nullptr;
	}
}


void LayerManager::PushLayer(Layer* Instance)
{
	if (!Instance)
		return;

	Instance->privAttached();
	layers.push_back(Instance);
}

void LayerManager::RemoveLayer(const Layer* Instance)
{
	const size_t size = layers.size();
	for (size_t i = 0; i < size; i++)
	{
		if (layers[i] == Instance)
		{
			layers[i]->privRemoved();
			layers[i] = nullptr;
		}
	}
}

void LayerManager::RemoveLayer(const String Name)
{
	const size_t size = layers.size();
	for (size_t i = 0; i < size; i++)
	{
		if (layers[i]->GetName() == Name)
		{
			layers[i]->privRemoved();
			layers[i] = nullptr;
		}
	}
}

void LayerManager::UpdateLayer()
{
	systemLayer->Update();

	for (Layer*& indexed : layers)
	{
		indexed->Update();
	}
}

static bool bWasAdded = false;
void LayerManager::PushSystemLayer()
{
	if (bWasAdded) return;

#ifdef _WIN64
	systemLayer = new WindowsLayer("Windows System Layer");
	systemLayer->Init();
#endif // _WIN64

	bWasAdded = true;
}

LayerManager::~LayerManager()
{

}
