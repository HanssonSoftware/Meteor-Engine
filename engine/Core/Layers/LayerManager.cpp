/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

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
		if (indexed)
		{
			indexed->Removed();
			indexed = nullptr;
		}
	}
}


void LayerManager::PushLayer(Layer* Instance)
{
	if (!Instance)
		return;

	Instance->Attached();
	layers.Add(Instance);
}

void LayerManager::RemoveLayer(const Layer* Instance)
{
	const uint32_t size = layers.GetSize();
	for (uint32_t i = 0; i < size; i++)
	{
		if (layers[i] == Instance)
		{
			layers[i]->Removed();
			layers[i] = nullptr;
		}
	}
}

void LayerManager::RemoveLayer(const String Name)
{
	const uint32_t size = layers.GetSize();
	for (uint32_t i = 0; i < size; i++)
	{
		if (layers[i]->GetName() == Name)
		{
			layers[i]->Removed();
			layers[i] = nullptr;
		}
	}
}

void LayerManager::UpdateLayer()
{
	systemLayer->Update();

	for (Layer*& indexed : layers)
	{
		if (indexed != nullptr) indexed->Update();
	}
}

static bool bWasAdded = false;
void LayerManager::PushSystemLayer()
{
	if (bWasAdded) 
		return;

#ifdef _WIN64
	systemLayer = new WindowsLayer("Windows System Layer");
	systemLayer->Init();
#endif // _WIN64

	bWasAdded = true;
}

LayerManager::~LayerManager()
{
	systemLayer->Removed();
	delete systemLayer;

	bWasAdded = false;
	systemLayer = nullptr;
}
