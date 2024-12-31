/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "FontAsset.h"
#include <d3d11.h>
#include <Graphics/D3D11/Direct3DDevice.h>
#include <Application/Application.h>
#include <Window/WindowManager.h>

static FT_Library library;

FontAsset::FontAsset(const char* fileName)
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{

	}

	error = FT_New_Face(library, "RobotoCondensed-Bold.ttf", 0, &faceObject);
	if (error == FT_Err_Unknown_File_Format)
	{
		throw;
	}
	else if (error)
	{
		throw;
	}

	if ((faceObject->face_flags & FT_FACE_FLAG_SCALABLE) != FT_FACE_FLAG_SCALABLE)
	{
		throw;
	}

	if (FT_Set_Pixel_Sizes(faceObject, 0, 64))
	{
		throw;
	}	
	
	if (FT_Set_Char_Size(faceObject, 0, 16*64, 640, 480))
	{
		throw;
	}	
	
	if (FT_Load_Glyph(faceObject, 0, FT_LOAD_DEFAULT))
	{
		throw;
	}	
	
	if (FT_Render_Glyph(faceObject->glyph, FT_Render_MODE_NORMAL))
	{
		throw;
	}

	FT_Bitmap& bom = faceObject->glyph->bitmap;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = bom.width;
	textureDesc.Height = bom.rows;               
	textureDesc.MipLevels = 1;                     
	textureDesc.ArraySize = 1;                         
	textureDesc.Format = DXGI_FORMAT_R8_UNORM;        
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;           
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = bom.buffer;
	initData.SysMemPitch = bom.pitch;

	ID3D11Texture2D* texture;
	auto temp = (IDirect3DDevice*)Application::Get()->getWindowManager()->getRenderContext();
	MR_ASSERT(temp, TEXT("Invalid Device!"));
	
	ID3D11Device* Device = temp->getDevice();

	HRESULT Result = Device->CreateTexture2D(&textureDesc, &initData, &texture);

	ID3D11ShaderResourceView* resView;
	HRESULT ResultA = Device->CreateShaderResourceView(texture, NULL, &resView);

	resView->Release();
	texture->Release();
}

FontAsset::~FontAsset()
{
	if (buffer)
	{
		//sft_freefont((SFT_Font*)buffer);
	}
}
