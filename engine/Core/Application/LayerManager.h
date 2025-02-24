/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <vector>
#include <Object/Object.h>

class Layer;
class String;

class LayerManager : public Object
{
	friend class Layer;
public:
	void addLayer(Layer* Instance);

	void removeLayer(const Layer* Instance);

	void removeLayer(const String Name);

	void updateLayer();

	~LayerManager();
private:
	std::vector<Layer*> layers;
};

