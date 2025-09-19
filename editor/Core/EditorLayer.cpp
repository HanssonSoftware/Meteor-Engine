/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "EditorLayer.h"
#include <Common/Guid.h>

EditorLayer::EditorLayer(const String& newName) noexcept
	: Layer(newName)
{
}

void EditorLayer::Init()
{

}

void EditorLayer::Attached()
{
	Layer::Attached();
}

void EditorLayer::Removed()
{
	Layer::Removed();
}

void EditorLayer::Update()
{

}
