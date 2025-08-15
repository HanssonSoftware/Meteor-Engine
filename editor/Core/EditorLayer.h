/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Layers/Layer.h>

#include "EditorCore.h"

class MR_EDITORAPPLICATION_API EditorLayer : public Layer
{
public:
	EditorLayer(const String newName) noexcept;

	virtual void Init() override;

	virtual void Attached() override;

	virtual void Removed() override;

	virtual void Update() override;
};

