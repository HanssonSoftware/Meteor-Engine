/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#pragma once
#include "../GraphicsDevice.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Log/LogMacros.h>
#include <d3d11.h>
#include <Types/Vector.h>

LOG_ADDCATEGORY(D3D11);



constexpr const FLOAT defaultClearColor[4] = { 0.f ,0.f ,0.f ,0.f };

class IDX11Device : public IGraphicsDevice
{
public:
	IDX11Device();

	virtual bool render() override;

	virtual void init() override;
	
	virtual void cleanUp() noexcept override;

	const virtual std::wstring getRendererSignatature() const override;

	virtual void updateSize(Vector2<uint32> newSize) override;

	virtual void setIsFullScreen(bool NewValue) override;
private:
	inline void createReferenceDevice();

	inline void createSwapChain(Vector2<uint32> newSize);

	inline void compileShader(const wchar_t* Path);

	inline void createFactory();

	inline void initTestScene();

	inline void createDepthStencil();

	inline void createViewport(Vector2<uint32> size);

	inline void createRenderTarget();

	inline void createVertexBuffer();

	inline void createInputLayout();

	ID3D11VertexShader* compiledVertexShader;

	ID3D11PixelShader* compiledPixelShader;

	ID3D11Device* device;

	IDXGIFactory* factory;

	IDXGISwapChain* swapChain;

	ID3D11DepthStencilView* stencilView;

	ID3D11RenderTargetView* renderTarget;

	ID3D11InputLayout* inputLayout;

	ID3D11Texture2D* backBuffer;

	ID3D11DeviceContext* deviceContext;

	Vector2<uint32> viewportSize;

	ID3D11Buffer* vertexBuffer;
};

