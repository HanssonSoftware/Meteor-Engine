/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include "../GraphicsDevice.h"

#define WIN32_LEAN_AND_MEAN
#include <Platform/Microsoft/MinWin.h>
#include <Log/LogMacros.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <Types/Vector.h>

LOG_ADDCATEGORY(D3D11);

struct constantWorld
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX proj;
	DirectX::XMMATRIX ortho;
};

constexpr const float SCREEN_DEPTH = 1000.0f;
constexpr const float SCREEN_NEAR = 0.3f;
constexpr const FLOAT defaultClearColor[4] = { 0.f ,0.f ,0.f ,0.f };

class IDirect3DDevice : public IGraphicsDevice
{
public:
	IDirect3DDevice();

	virtual bool Render(float deltaTime) override;

	virtual bool Init() override;
	
	virtual void cleanUp() noexcept override;

	const virtual String getRendererSignatature() const override;

	virtual void resizeBuffers(Vector2<uint32> newSize) override;

	virtual void setIsFullScreen(bool NewValue) override;

	bool buseWframe = 0;

	void changeDisplayMode();

	/** Gets the Direct3D 11 device, used for creating resources and managing GPU tasks. */
	ID3D11Device* getDevice() const { return device; };

	/** Gets the Direct3D 11 device context, primarily used for rendering commands. */
	ID3D11DeviceContext* getDeviceContext() const { return deviceContext; };

	/** Gets the compiled vertex shader, which handles vertex transformation logic. */
	ID3D11VertexShader* getCompiledVertexShader() const { return compiledVertexShader; };

	/** Gets the compiled pixel shader, used for pixel-level shading and effects. */
	ID3D11PixelShader* getCompiledPixelShader() const { return compiledPixelShader; };

	/** Gets the input layout, mapping vertex buffer data to the vertex shader. */
	ID3D11InputLayout* getInputLayout() const { return inputLayout; };

	/** Gets the rasterizer state, managing how primitives are rendered (e.g., culling). */
	ID3D11RasterizerState* getRasterizerState() const { return rasterizerState; };

	/** Gets the rasterizer state for wireframe rendering. */
	ID3D11RasterizerState* getRasterizerStateWireFrame() const { return rasterizerStateWireFrame; };

	/** Gets the Direct3D 11 depth stencil view, used for depth testing. */
	ID3D11DepthStencilView* getDepthStencil() const { return depthStencil; };

	/** Gets the depth stencil texture resource. */
	ID3D11Texture2D* getDepthStencilTex() const { return depthStencilTex; };

	/** Gets the Direct3D 11 render target view, representing the back buffer. */
	ID3D11RenderTargetView* getRenderTarget() const { return RenderTarget; };

	/** Gets the Direct3D 11 sampler state, used for sampling textures. */
	ID3D11SamplerState* getSamplerState() const { return samplerState; };

	/** Gets the Direct3D 11 depth stencil state, managing depth and stencil operations. */
	ID3D11DepthStencilState* getDepthStencilState() const { return depthStencilState; };

	/** Gets the Direct3D 11 back buffer texture. */
	ID3D11Texture2D* getBackBuffer() const { return backBuffer; };

	/** Gets the swap chain, which manages the buffers for rendering. */
	IDXGISwapChain* getSwapChain() const { return swapChain; };

	/** Gets the DirectX graphics interface (DXGI) factory, used for creating adapters. */
	IDXGIFactory* getFactory() const { return factory; };

	/** Gets the selected DXGI adapter, representing the GPU in use. */
	IDXGIAdapter* getAdapter() const { return adapter; };

	ID3D11Buffer* getWorldBuffer() const { return worldBuffer; };

	void setWorldBuffer(ID3D11Buffer* newB) { worldBuffer = newB; };

	DirectX::XMMATRIX getWorldMatrix() const { return worldMatrix; };

	void setWorldMatrix(DirectX::XMMATRIX matrix) { worldMatrix = matrix; };

private:

	inline void createReferenceDevice();

	inline void createSwapChain(Vector2<uint32> newSize);

	inline void compileShader(String vertexShader, String pixelShader);

	inline void createFactory();

	inline void createRasterizerState();

	inline void InitTestScene();

	inline void calculateNumerics(Vector2<uint32> size);

	inline void createDepthStencil(Vector2<uint32> size);

	inline void createViewport(Vector2<uint32> size);

	inline void createRenderTarget();

	inline void createVertexBuffer();

	inline void enumAdapters();

	inline void imguiStateChanged(bool NewValue);

	inline void createInputLayout();

	inline void createSampler();

	inline const String distinguishFeatureLevel(D3D_FEATURE_LEVEL featureLevel) const;

	float screenAspect;

	DirectX::XMMATRIX worldMatrix;

	DirectX::XMMATRIX projectionMatrix;

	DirectX::XMMATRIX orthoMatrix;

	ID3D11Device* device;

	ID3D11VertexShader* compiledVertexShader;

	ID3D11PixelShader* compiledPixelShader;

	ID3D11InputLayout* inputLayout;

	ID3D11RasterizerState* rasterizerState;

	ID3D11RasterizerState* rasterizerStateWireFrame;

	IDXGIFactory* factory;
	
	IDXGIAdapter* adapter;

	IDXGISwapChain* swapChain;

	ID3D11DepthStencilView* depthStencil;
	ID3D11Texture2D* depthStencilTex;

	ID3D11RenderTargetView* RenderTarget;

	ID3D11SamplerState* samplerState;

	ID3D11DepthStencilState* depthStencilState;

	ID3D11Texture2D* backBuffer;

	ID3D11DeviceContext* deviceContext;

	ID3D11Buffer* worldBuffer;

	int selectedAdapter = -1;
	std::vector<IDXGIAdapter*> availableAdapters;
};

