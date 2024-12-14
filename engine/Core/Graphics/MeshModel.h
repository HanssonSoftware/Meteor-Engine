/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <d3d11.h>
#include <Types/PlatformDefs.h>
#include <Types/Vector.h>
#include <Object/Object.h>
#include <Graphics/QueuedRender.h>

class String;
struct Vertex;

/** ModelData is restricted to DX11 only! */
typedef struct ModelData
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	uint32 indiciesCount = 0;
	uint32 verticiesCount = 0;

} ModelData;

class MeshModel : public Object, public IQueuedRender
{
public:
	MeshModel();

	~MeshModel();

	void Import(String name);

	ModelData data;

	void* model;

	void* device;

	// Inherited via IQueuedRender
	void Render() override;

	void bindRender() override;

	void unbindRender() override;

	std::vector<Vertex> vertexes;

	std::vector<int> indicies;
private:
	bool bWasSuccessFullyCreated = false;

	ID3D11Buffer* res;
};

