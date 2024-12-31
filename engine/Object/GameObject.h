/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "Object.h"
#include <Graphics/QueuedRender.h>
#include <vector>
#include <DirectXMath.h>

class GameObject : public Object, public IQueuedRender
{
	void Render() override;

	void updateMatrices(const DirectX::XMMATRIX parent);

	void bindRender() override;

	void unbindRender() override;

	DirectX::XMMATRIX positionMatrix;

	std::vector<GameObject*> children;
};

