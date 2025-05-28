/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <vector>
#include <Object/Object.h>

class Layer;
struct String;
class OSLayer;

class LayerManager : public Object
{
	friend class Layer;
public:
	LayerManager();

	virtual ~LayerManager();

	void PushLayer(Layer* Instance);

	void RemoveLayer(const Layer* Instance);

	void RemoveLayer(const String Name);

	void UpdateLayer();

	void PushOSLayer();

	virtual void Init() override;

	virtual void Shutdown() override;

	OSLayer* GetSystemLayer() const { return systemLayer ? systemLayer : nullptr; }
private:
	OSLayer* systemLayer;

	std::vector<Layer*> layers;
};

