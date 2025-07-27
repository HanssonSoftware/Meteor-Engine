/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <ThirdParty/FreeType/ft2build.h>
//#include <d3d11.h>

#include FT_FREETYPE_H

class FontAsset
{
public:
	FontAsset(const char* fileName);

	~FontAsset();

	void* buffer = nullptr;

private:

	FT_Face faceObject;
};

