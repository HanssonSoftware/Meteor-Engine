/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include "Importer.h"
#include <d3d11.h>

class FontImporter : public IImporter
{
public:
	FontImporter(const String Name);

	void Read();

	ID3D11Texture2D* texture;

	ID3D11Buffer* buffer;

	// Inherited via IImporter
	void loadAsset(const String Name) override;
};

