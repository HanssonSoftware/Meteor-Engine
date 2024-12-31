/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "SceneGraph.h"
#include <Graphics/D3D11/Direct3DDevice.h>
#include <Application/Application.h>
#include <Window/WindowManager.h>


SceneGraph::SceneGraph()
{
	worldMatrix = DirectX::XMMatrixIdentity();
	usedRHI = (IDirect3DDevice*)Application::Get()->getWindowManager()->getRenderContext();
}

void SceneGraph::Update(float deltaTime)
{
	for (Object*& Obj : sceneList)
	{
		Obj->Frame(deltaTime);
	}
}

void SceneGraph::addToRoot(Object* objectToAdd)
{
	if (!objectToAdd)
	{
		MR_LOG(LogSceneGraph, Error, TEXT("Invalid Object!"));
		return;
	}

	sceneList.push_back(objectToAdd);
}

SceneGraph& SceneGraph::Get()
{
	static SceneGraph instance;
	return instance;
}
