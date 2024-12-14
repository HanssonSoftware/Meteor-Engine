/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <vector>
#include <Object/Object.h>
#include <Log/LogMacros.h>

#include <DirectXMath.h>

class IDirect3DDevice;

LOG_ADDCATEGORY(SceneGraph);

class SceneGraph : public Object
{
	friend class IGraphicsDevice;

public:
	SceneGraph();

	void Update(float deltaTime);

	void addToRoot(Object* objectToAdd);

	std::vector<Object*> sceneList;

	static SceneGraph& Get();

	DirectX::XMMATRIX getWorldMatrix() const
	{
		return worldMatrix;
	}

private:
	DirectX::XMMATRIX worldMatrix;

	IDirect3DDevice* usedRHI;
};

