/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "GameObject.h"

void GameObject::Render()
{
	for (const auto& child : children)
	{
		child->updateMatrices(positionMatrix);
	}
}

void GameObject::updateMatrices(const DirectX::XMMATRIX parent)
{
	positionMatrix = positionMatrix * parent;
}

void GameObject::bindRender()
{
}

void GameObject::unbindRender()
{
}
