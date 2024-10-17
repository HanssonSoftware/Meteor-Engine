/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#include "DX11.h"
#include <Application/Application.h>
#include <Log/LogMacros.h>
#include <chrono>
#include <Types/PlatformDefs.h>
#include <d3d11sdklayers.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

Vertex vertices[] =
{
	{ { 0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // Felsõ csúcs, piros
	{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // Jobb alsó, zöld
	{ {-0.5f, -0.5f, 0.0f }, { 0.14f, 0.14f, 0.16f, 1.0f } }  // Bal alsó, kék
};

IDX11Device::IDX11Device()
{
	device = nullptr;

	deviceContext = nullptr;

	factory = nullptr;

	swapChain = nullptr;

	renderTarget = nullptr;

	backBuffer = nullptr;

	stencilView = nullptr;

	inputLayout = nullptr;

	compiledPixelShader = nullptr;

	compiledVertexShader = nullptr;

	vertexBuffer = nullptr;
}

static constexpr const FLOAT ClearColor[] = {0.13f , 0.14f, 0.17f, 1.f};
bool IDX11Device::render()
{
	if (!IGraphicsDevice::render())
		return false;

	deviceContext->ClearRenderTargetView(renderTarget, ClearColor);

	deviceContext->OMSetRenderTargets(1, &renderTarget, nullptr);

	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;
	//deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//deviceContext->IASetInputLayout(inputLayout);
	//deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//deviceContext->VSSetShader(compiledVertexShader, nullptr, 0);
	//deviceContext->PSSetShader(compiledPixelShader, nullptr, 0);


	//deviceContext->Draw(3, 0);

	swapChain->Present(0, 0);

	return true;
}

void IDX11Device::init()
{
	createReferenceDevice();

	createFactory();


	createSwapChain(Application::Get().getWindowSize());

	createRenderTarget();

	//createDepthStencil();
	initTestScene();

	createVertexBuffer();
	
	createViewport(Application::Get().getWindowSize());

	//compileShader(L"A");

	setIsFullScreen(false);

	bIsDriverInitialized = true;
	setDriverReadyState(true);
}

void IDX11Device::cleanUp() noexcept
{
	swapChain->SetFullscreenState(false, NULL);

	if (device)
	{
		device->Release();
		device = nullptr;
	}	
	
	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}	

	if (factory)
	{
		factory->Release();
		factory = nullptr;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}

	if (renderTarget)
	{
		renderTarget->Release();
		renderTarget = nullptr;
	}	
	
	if (backBuffer)
	{
		backBuffer->Release();
		backBuffer = nullptr;
	}	

	if (stencilView)
	{
		stencilView->Release();
		stencilView = nullptr;
	}	
	
	if (compiledVertexShader)
	{
		compiledVertexShader->Release();
		compiledVertexShader = nullptr;
	}	
	
	if (inputLayout)
	{
		inputLayout->Release();
		inputLayout = nullptr;
	}
}

const std::wstring IDX11Device::getRendererSignatature() const
{
	return L"DX11";
}

static constexpr uint32 threshold = 20;
void IDX11Device::updateSize(Vector2<uint32> newSize)
{
	if (!(newSize > 10))
		return;

	if (deviceContext) {
		deviceContext->OMSetRenderTargets(0, NULL, NULL);
	}

	if (renderTarget) {
		renderTarget->Release();
		renderTarget = nullptr;
	}

	if (backBuffer) {
		backBuffer->Release();
		backBuffer = nullptr;
	}

	if (stencilView) {
		stencilView->Release();
		stencilView = nullptr;
	}

	HRESULT A = swapChain->ResizeBuffers(2, newSize.x, newSize.y, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

#pragma warning(disable : 6387) // disable the fucking X can be 0.
	HRESULT B = device->CreateRenderTargetView(backBuffer, NULL, &renderTarget);

	createViewport(newSize);

	static Vector2<uint32> lastLoggedSize;

	// Calculate the deltas
	const int deltaX = std::abs(static_cast<int>(newSize.x) - static_cast<int>(lastLoggedSize.x));
	const int deltaY = std::abs(static_cast<int>(newSize.y) - static_cast<int>(lastLoggedSize.y));

	// If either delta is bigger than or lower, it will set and log the last size
	if (deltaX > threshold || deltaY > threshold)
	{
		lastLoggedSize = newSize; // Lastly update the size
		MR_LOG(LogD3D11, Log, TEXT("Viewport resized to: X=%d\tY=%d"), newSize.x, newSize.y);
	}
}

void IDX11Device::setIsFullScreen(bool NewValue)
{
	IGraphicsDevice::setIsFullScreen(NewValue);
	HWND window = (HWND)Application::Get().getWindowHandle();

	swapChain->SetFullscreenState(NewValue ? TRUE : FALSE, NULL);

	DXGI_MODE_DESC swInfo = {};
	swInfo.Width = NewValue ? 1920 : 1280;
	swInfo.Height = NewValue ? 1080 : 720;
	swInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	swapChain->ResizeTarget(&swInfo);
}

void IDX11Device::createReferenceDevice()
{
	// First insight: leave the flags 0.
	UINT deviceFlags = 0;

#ifdef MR_DEBUG
	// If we use debug configuration, why not enable the debugging part.
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // MR_DEBUG

	// Give a nice feature level list, it will go from top to the bottom, if neither supported it will warns the user.
	constexpr const D3D_FEATURE_LEVEL featureLevels[]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	// Burn in the quantity of the list
	constexpr const uint32 featureLevelsQuantity = std::size(featureLevels);

	// At last, create the device.
	HRESULT result = D3D11CreateDevice(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 
		deviceFlags,
		featureLevels, 
		featureLevelsQuantity, 
		D3D11_SDK_VERSION, 
		&device, 
		0, 
		&deviceContext
	);

	MR_ASSERT(result == S_OK, TEXT("D3D11CreateDevice failed! Check with Debug configuration!"));
}

inline void IDX11Device::createSwapChain(Vector2<uint32> newSize)
{
	MR_ASSERT(device, TEXT("device is invalid!"));

	MR_ASSERT(newSize.x > 0 || newSize.y > 0, TEXT("Invalid Swapchain size!"));

	// Get system metrics. Output is: s=1920 | y=1080, from one of my monitors.
	//auto s = GetSystemMetrics(SM_CXSCREEN);
	//auto y = GetSystemMetrics(SM_CYSCREEN);

	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.Windowed = true;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	scDesc.BufferCount = 2;
	scDesc.BufferDesc.RefreshRate.Denominator = 165;
	scDesc.BufferDesc.RefreshRate.Numerator = 1;
	scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferDesc.Width = newSize.x;
	scDesc.BufferDesc.Height = newSize.y;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	scDesc.SampleDesc.Quality = 0;
	scDesc.SampleDesc.Count = 1;

	scDesc.OutputWindow = (HWND)Application::Get().getWindowHandle();

	HRESULT result = factory->CreateSwapChain(device, &scDesc, &swapChain);
	MR_ASSERT(result == S_OK, TEXT("CreateSwapChain failed! Check with Debug configuration!"));
}

inline void IDX11Device::compileShader(const wchar_t* Path)
{
	ID3DBlob* vertexBlob;
	ID3DBlob* errorMsg;
	HRESULT result = D3DCompileFromFile(L"Shader.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexBlob, &errorMsg);
	MR_ASSERT(result == S_OK, TEXT("D3DCompileFromFile (Shader) failed! Check with Debug configuration!"));


	ID3DBlob* pixelBlob;
	HRESULT resultA = D3DCompileFromFile(L"Shader.px", NULL, NULL, "PS", "ps_5_0", 0, 0, &pixelBlob, &errorMsg);
	MR_ASSERT(resultA == S_OK, TEXT("D3DCompileFromFile (Pixel) failed! Check with Debug configuration!"));

	if (errorMsg)
		errorMsg->Release();

	MR_LOG(LogD3D11, Log, TEXT("Compiling shader(s)!"));

	HRESULT resultB = device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, &compiledVertexShader);
	MR_ASSERT(resultB == S_OK, TEXT("CreateVertexShader failed! Check with Debug configuration!"));

	HRESULT resultC = device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, &compiledPixelShader);
	MR_ASSERT(resultC == S_OK, TEXT("CreatePixelShader failed! Check with Debug configuration!"));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	constexpr UINT numElements = std::size(layout);

	HRESULT resultD = device->CreateInputLayout(layout, numElements, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &inputLayout);
	MR_ASSERT(resultD == S_OK, TEXT("CreateInputLayout failed! Check with Debug configuration!"));

	vertexBlob->Release();
	pixelBlob->Release();
}

inline void IDX11Device::createFactory()
{
	// Create the factory here.
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	// Aaaand disable the Alt+Enter. It doesn't work.
	factory->MakeWindowAssociation((HWND)Application::Get().getWindowHandle(), DXGI_MWA_NO_ALT_ENTER);

	MR_ASSERT(result == S_OK, TEXT("CreateDXGIFactory failed! Check with Debug configuration!"));
}

inline void IDX11Device::initTestScene()
{

}

inline void IDX11Device::createDepthStencil()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewInfo = {};
	stencilViewInfo.Format = DXGI_FORMAT_D16_UNORM;
	stencilViewInfo.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	HRESULT result = device->CreateDepthStencilView(backBuffer, &stencilViewInfo, &stencilView);
	MR_ASSERT(result == S_OK, TEXT("CreateDepthStencilView failed! Check with Debug configuration!"));
}

inline void IDX11Device::createViewport(Vector2<uint32> size)
{
	D3D11_VIEWPORT vpInfo = {};
	vpInfo.TopLeftX = 0;
	vpInfo.TopLeftY = 0;
	vpInfo.Width = (FLOAT)size.x;
	vpInfo.Height = (FLOAT)size.y;
	vpInfo.MaxDepth = 1.f;
	vpInfo.MinDepth = 0.f;

	deviceContext->RSSetViewports(1, &vpInfo);
}

inline void IDX11Device::createRenderTarget()
{
	MR_ASSERT(swapChain, TEXT("swapChain is invalid!"));

	D3D11_RENDER_TARGET_VIEW_DESC rtargetViewDesc = {};

	HRESULT result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	MR_ASSERT(result == S_OK, TEXT("CreateRenderTargetView failed! Check with Debug configuration!"));

	HRESULT resultA = device->CreateRenderTargetView(backBuffer, NULL, &renderTarget);
	MR_ASSERT(resultA == S_OK, TEXT("CreateRenderTargetView failed! Check with Debug configuration!"));

	deviceContext->OMSetRenderTargets(1, &renderTarget, NULL);
}

inline void IDX11Device::createVertexBuffer()
{
	D3D11_BUFFER_DESC bufferInfo = {};
	bufferInfo.Usage = D3D11_USAGE_DEFAULT;
	bufferInfo.CPUAccessFlags = 0;
	bufferInfo.ByteWidth = sizeof(vertices);
	bufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initInfo = {};
	initInfo.pSysMem = vertices;

	HRESULT result = device->CreateBuffer(&bufferInfo, &initInfo, &vertexBuffer);
	MR_ASSERT(result == S_OK, TEXT("CreateBuffer failed! Check with Debug configuration!"));
}

inline void IDX11Device::createInputLayout()
{

}


