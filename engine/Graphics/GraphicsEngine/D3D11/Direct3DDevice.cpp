/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Direct3DDevice.h"
#include <Application/Application.h>
//#include <dwmapi.h>
#include <Log/LogMacros.h>
//#include <Types/Cast.h>
#include <Window/Window.h>
//#include <Types/PlatformDefs.h>
//#include <d3d11sdklayers.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Log/Exception.h>
#include <GraphicsEngine/QueuedRender.h>
#include <GraphicsEngine/MeshModel.h>
#include <Window/WindowManager.h>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
//#include <mutex>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxcompiler.lib")

#define HRESULT_STRINGIZE(code) #code

IDirect3DDevice::IDirect3DDevice()
{
	device = nullptr;

	deviceContextDEF = nullptr;

	deviceContextIMM = nullptr;

	factory = nullptr;

	swapChain = nullptr;

	RenderTarget = nullptr;

	backBuffer = nullptr;

	depthStencil = nullptr;

	inputLayout = nullptr;

	compiledPixelShader = nullptr;

	compiledVertexShader = nullptr;

}

static DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
static constexpr const FLOAT ClearColor[] = {0.13f , 0.14f, 0.17f, 1.f};
//static std::mutex RenderingMutex;
bool IDirect3DDevice::Render()
{
	if (getDeviceReadyState() != GRAPHICS_ENGINE_STATE_RUNNING)
		return false;

	static constexpr const FLOAT array[4] = {0.f, 0.1f, 0.1f, 1.f};
	deviceContextIMM->ClearRenderTargetView(RenderTarget, array);
	//deviceContextIMM->ClearDepthStencilView(depthStencil, D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.f, UINT8(0));
	deviceContextIMM->OMSetRenderTargets(1, &RenderTarget, depthStencil);


	if (getImGUIUsed())
	{
		//ImGui_ImplDX11_NewFrame();
		//ImGui_ImplWin32_NewFrame();
		//ImGui::NewFrame();
		Application::Get()->nc.initDebug();
	}

	{
		constantWorld w;
		w.projection = DirectX::XMMatrixTranspose(projectionMatrix);
		w.world = DirectX::XMMatrixTranspose(worldMatrix);
		w.view = DirectX::XMMatrixTranspose(Application::Get()->nc.getViewMatrix());

		deviceContextIMM->UpdateSubresource(worldBuffer, 0, 0, &w, 0, 0);
	}


	for (Object* sceneObject : graphToRender->sceneList)
	{
		if (IQueuedRender* Renderable = dynamic_cast<IQueuedRender*>(sceneObject))
		{
			Renderable->Render();
		}
	}


	if (getImGUIUsed())
	{
		//ImGui::Render();
		//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	//deviceContextDEF->FinishCommandList(0, &commandList);
	//deviceContextIMM->ExecuteCommandList(commandList, 0);

	HRESULT	presentResult = swapChain->Present(0, 0);

	if (presentResult == DXGI_ERROR_DEVICE_RESET)
	{
		getDescriptiveError(device->GetDeviceRemovedReason());
		cleanUp();
		Init();
		return false;
	}
	else if (presentResult == DXGI_ERROR_DEVICE_REMOVED)
	{
		getDescriptiveError(device->GetDeviceRemovedReason());
		cleanUp();
		Init();
		return false;
	}

	if (getImGUIUsed())
	{
		//ImGui::EndFrame();
	}

	//deviceContextIMM->OMSetRenderTargets(0, 0, 0);
	return true;
}

static constexpr float fieldOfView = 3.141592654f / 4.0f;
bool IDirect3DDevice::Init()
{
	//std::this_thread::sleep_for(std::chrono::seconds(10));
	IGraphicsDevice::Init();

	imguiValueChange.subscribe(&IDirect3DDevice::imguiStateChanged, this);

	const Vector2<uint32> defaultWindowSize = Application::Get()->getWindowSize();

	screenAspect = (float)defaultWindowSize.x / (float)defaultWindowSize.y;

	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 10000.f);

	orthoMatrix = DirectX::XMMatrixOrthographicLH((float)defaultWindowSize.x, (float)defaultWindowSize.y, 0.1f, 10000.f);


	createFactory();

	enumAdapters();

	createReferenceDevice();

	createSwapChain(defaultWindowSize);

	createDepthStencil(defaultWindowSize);

	createRenderTarget();

	createViewport(defaultWindowSize);

	calculateNumerics(defaultWindowSize);

	//createVertexBuffer();

	compileShader("Shader", "Shader");

	createRasterizerState();

	//createSampler();

	setIsFullScreen(false);

	{

		D3D11_BUFFER_DESC bf = {};
		bf.Usage = D3D11_USAGE_DEFAULT;
		bf.ByteWidth = sizeof(constantWorld);
		bf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bf.CPUAccessFlags = 0;

		HRESULT A = device->CreateBuffer(&bf, 0, &worldBuffer);
	}

	setImGUIUsed(true);
	setDeviceReadyState(GRAPHICS_ENGINE_STATE_RUNNING);
 	return true;
}

void IDirect3DDevice::cleanUp() noexcept
{
	setDeviceReadyState(GRAPHICS_ENGINE_STATE_SHUTDOWN);
	
	if (getImGUIUsed())
	{
		//ImGui_ImplDX11_Shutdown();
		//ImGui_ImplWin32_Shutdown();
		//ImGui::DestroyContext();
	}

	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
		swapChain->Release();
		swapChain = nullptr;
	}

	if (device)
	{
		device->Release();
		device = nullptr;
	}		
		
	if (deviceContextIMM)
	{
		deviceContextIMM->ClearState();
		deviceContextIMM->Release();
		deviceContextIMM = nullptr;
	}	

	if (factory)
	{
		factory->Release();
		factory = nullptr;
	}

	if (RenderTarget)
	{
		RenderTarget->Release();
		RenderTarget = nullptr;
	}	
	
	if (backBuffer)
	{
		backBuffer->Release();
		backBuffer = nullptr;
	}	

	if (depthStencil) {
		depthStencil->Release();
		depthStencil = nullptr;
	}

	if (depthStencilTex) {
		depthStencilTex->Release();
		depthStencilTex = nullptr;
	}

	if (depthStencilState) {
		depthStencilState->Release();
		depthStencilState = nullptr;
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

const String IDirect3DDevice::getRendererSignatature() const
{
	return L"DX11";
}

static constexpr uint32 threshold = 20;
void IDirect3DDevice::resizeBuffers(Vector2<uint32> newSize)
{
	setDeviceReadyState(GRAPHICS_ENGINE_STATE_PAUSED_RENDERING);

	if (deviceContextIMM) {
		deviceContextIMM->OMSetRenderTargets(0, NULL, NULL);
	}

	if (RenderTarget) {
		RenderTarget->Release();
		RenderTarget = nullptr;
	}

	if (backBuffer) {
		backBuffer->Release();
		backBuffer = nullptr;
	}

	if (depthStencil) {
		depthStencil->Release();
		depthStencil = nullptr;
	}	
	
	if (depthStencilTex) {
		depthStencilTex->Release();
		depthStencilTex = nullptr;
	}

	if (depthStencilState) {
		depthStencilState->Release();
		depthStencilState = nullptr;
	}	

	HRESULT A = swapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

	createDepthStencil(newSize);

	createRenderTarget();

	deviceContextIMM->RSSetViewports(0, 0);
	createViewport(newSize);

	setDeviceReadyState(GRAPHICS_ENGINE_STATE_RUNNING);
}

bool IDirect3DDevice::setIsFullScreen(bool NewValue)
{
	if (!IGraphicsDevice::setIsFullScreen(NewValue))
		return false;

	setDeviceReadyState(GRAPHICS_ENGINE_STATE_PAUSED_RENDERING);

	HWND window = (HWND)Application::Get()->getWindowManager()->getFirstWindow()->getWindowHandle();

	swapChain->SetFullscreenState(NewValue ? TRUE : FALSE, NULL);

	//if (NewValue)
	//{
	//	SetWindowTheme(window, L"", L"");
	//}

	DXGI_MODE_DESC swInfo = {};
	swInfo.Width = NewValue ? 1920 : 1280;
	swInfo.Height = NewValue ? 1080 : 720;
	swInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	swapChain->ResizeTarget(&swInfo);
	setDeviceReadyState(GRAPHICS_ENGINE_STATE_RUNNING);
	return true;
}

void IDirect3DDevice::changeDisplayMode()
{
	static uint32 modes = 0;

	if (modes >= 2)
	{
		modes = 0;
		buseWframe = false;
		return;
	}

	modes++;
	buseWframe = true;
}

inline String IDirect3DDevice::getDescriptiveError(const HRESULT result)
{
	if (GetLastError() == S_OK)
		return String();

	wchar_t* Temp;
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		0,
		(LPWSTR)&Temp,
		0,
		NULL
	);

	return Temp;
}

void IDirect3DDevice::createReferenceDevice()
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
		D3D_FEATURE_LEVEL_11_0
	};

	// Burn in the quantity of the list
	constexpr const uint32 featureLevelsQuantity = std::size(featureLevels);

	// At last, create the device.
	HRESULT result = D3D11CreateDevice(
		availableAdapters[selectedAdapter],
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		deviceFlags,
		featureLevels,
		featureLevelsQuantity,
		D3D11_SDK_VERSION,
		&device,
		&usedfeatureLevel,
		&deviceContextIMM
	);

	if (result == DXGI_ERROR_UNSUPPORTED)
	{
		MR_LOG(LogD3D11, Warn, TEXT("No discrete graphics card detected, launching with WARP!"));
		result = D3D11CreateDevice(
			availableAdapters[selectedAdapter],
			D3D_DRIVER_TYPE_WARP,
			NULL,
			deviceFlags,
			featureLevels,
			featureLevelsQuantity,
			D3D11_SDK_VERSION,
			&device,
			0,
			&deviceContextIMM
		);
	}

	if (result != S_OK)
	{
		MR_LOG(LogD3D11, Fatal, TEXT("Failed to Create D3D11 Capable Device"));
	}

	static bool bWasInitiatedBefore;
	if (Logger::Get().getLoggingLevel() & APPFLAG_ENABLE_VERBOSE_LOGGING && !bWasInitiatedBefore)
	{
		bWasInitiatedBefore = true;
		DXGI_ADAPTER_DESC apInfo = {};

		adapter->GetDesc(&apInfo);
		MR_LOG(LogD3D11, Verbose, TEXT("Capable Device Found!"));
		MR_LOG(LogD3D11, Verbose, TEXT("\tName: %s"), apInfo.Description);
		MR_LOG(LogD3D11, Verbose, TEXT("\tAvailable Memory: %d MB"), apInfo.DedicatedVideoMemory >> 20);
		MR_LOG(LogD3D11, Verbose, TEXT("\tHighest Feature Level: %s"), distinguishFeatureLevel(device->GetFeatureLevel()).Chr());
	}


	MR_ASSERT(device != nullptr, TEXT("device is Invalid!"));

	//D3D11_FEATURE_DATA_THREADING thrd = {};
	//HRESULT chk = device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &thrd, sizeof(thrd));
	//if (thrd.DriverConcurrentCreates == 1)
	//{
	//	chk = device->CreateDeferredContext(0, &deviceContextDEF);
	//}

	if (getImGUIUsed())
	{
		//imguiStateChanged(getImGUIUsed());
	}
}

inline void IDirect3DDevice::createSwapChain(Vector2<uint32> newSize)
{
	//MR_LOG(LogD3D11, Fatal, TEXT("Invalid D3D11 device!"));

	if (newSize.x < 32/* && newSize.y < 32*/)
	{
		THROW_EXCEPTION("Invalid Swapchain size! X=%d Y=%d", newSize.x, newSize.y);
	}

	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.Windowed = true;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	scDesc.BufferCount = 2;
	scDesc.BufferDesc.RefreshRate.Denominator = 165;
	scDesc.BufferDesc.RefreshRate.Numerator = 1;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferDesc.Width = newSize.x;
	scDesc.BufferDesc.Height = newSize.y;
	scDesc.Flags = bIsFullScreenSwitchEnabled ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

	scDesc.SampleDesc.Quality = 0;
	scDesc.SampleDesc.Count = 1;

	scDesc.OutputWindow = (HWND)Application::Get()->getWindowManager()->getFirstWindow()->getWindowHandle();

	HRESULT result = factory->CreateSwapChain(device, &scDesc, &swapChain);
	if (FAILED(result))
	{
		THROW_EXCEPTION("CreateSwapChain failed! Check with Debug configuration!");
	}
}

inline void IDirect3DDevice::compileShader(String vertexShader, String pixelShader)
{
	ID3DBlob* vertexBlob;
	ID3DBlob* errorMsg;
	HRESULT result = D3DCompileFromFile((vertexShader + String(L".hlsl")).Chr(), NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexBlob, &errorMsg);
	if (result != S_OK)
	{
		if (errorMsg)
		{
			const char* message = (const char*)errorMsg->GetBufferPointer();
			MessageBoxA(0, message, "Compile Error!", MB_ICONERROR | MB_OK);
			errorMsg->Release();
		}
	}


	ID3DBlob* pixelBlob;
	result = D3DCompileFromFile((vertexShader + String(L"PX.hlsl")).Chr(), NULL, NULL, "PS", "ps_5_0", 0, 0, &pixelBlob, &errorMsg);
	if (result != S_OK)
	{
		if (errorMsg)
		{
			const char* message = (const char*)errorMsg->GetBufferPointer();
			MessageBoxA(0, message, "Compile Error!", MB_ICONERROR | MB_OK);
			errorMsg->Release();
		}
	}
	
	

	MR_LOG(LogD3D11, Log, TEXT("Compiling shader(s)!"));

	HRESULT resultB = device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, &compiledVertexShader);
	if (FAILED(resultB))
	{
		THROW_EXCEPTION("CreateVertexShader failed! Check with Debug configuration!");
	}

	HRESULT resultC = device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, &compiledPixelShader);
	if (FAILED(resultC))
	{
		THROW_EXCEPTION("CreatePixelShader failed! Check with Debug configuration!");
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	constexpr UINT numElements = std::size(layout);

	HRESULT resultD = device->CreateInputLayout(layout, numElements, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &inputLayout);
	if (FAILED(resultD))
	{
		THROW_EXCEPTION("CreateInputLayout failed! Check with Debug configuration!");
	}

	deviceContextIMM->IASetInputLayout(inputLayout);

	vertexBlob->Release();
	pixelBlob->Release();
}

inline void IDirect3DDevice::createFactory()
{
	// Create the factory here.
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	// Aaaand disable the Alt+Enter. It doesn't work.
	factory->MakeWindowAssociation((HWND)Application::Get()->getWindowManager()->getFirstWindow()->getWindowHandle(), DXGI_MWA_NO_ALT_ENTER);

	if (FAILED(result))
	{
		THROW_EXCEPTION("CreateDXGIFactory failed!Check with Debug configuration!");
	}
}

inline void IDirect3DDevice::createRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerInfo = {};
	rasterizerInfo.FillMode = D3D11_FILL_SOLID;
	rasterizerInfo.CullMode = D3D11_CULL_NONE;	
	
	D3D11_RASTERIZER_DESC rasterizerInfoWire = {};
	rasterizerInfoWire.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerInfoWire.CullMode = D3D11_CULL_NONE;
	
	HRESULT Result = device->CreateRasterizerState(&rasterizerInfo, &rasterizerState);
	if (FAILED(Result))
	{
		THROW_EXCEPTION("CreateRasterizerState failed! Check with Debug configuration!");
	}
	
	Result = device->CreateRasterizerState(&rasterizerInfoWire, &rasterizerStateWireFrame);
	if (FAILED(Result))
	{
		THROW_EXCEPTION("CreateRasterizerState failed! Check with Debug configuration!");
	}
}

inline void IDirect3DDevice::InitTestScene()
{

}

inline void IDirect3DDevice::calculateNumerics(Vector2<uint32> size)
{

}

inline void IDirect3DDevice::createDepthStencil(Vector2<uint32> size)
{
	static Vector2<uint32> cache;
	if (size != 0)
		cache = size;

	D3D11_TEXTURE2D_DESC stencilViewInfo = {};
	stencilViewInfo.Width = size.x == 0 ? cache.x : size.x;
	stencilViewInfo.Height = size.y == 0 ? cache.y : size.y;
	stencilViewInfo.MipLevels = 1;
	stencilViewInfo.ArraySize = 1;
	stencilViewInfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	stencilViewInfo.SampleDesc.Quality = 0;
	stencilViewInfo.SampleDesc.Count = 1;
	stencilViewInfo.Usage = D3D11_USAGE_DEFAULT;
	stencilViewInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	stencilViewInfo.CPUAccessFlags = 0;
	stencilViewInfo.MiscFlags = 0;

	// FIXME: Creating Depth Stencil causes memory leak!
	//HRESULT result = device->CreateTexture2D(&stencilViewInfo, NULL, &depthStencilTex);
	//if (FAILED(result))
	//{
	//	THROW_EXCEPTION("CreateDepthStencilView failed! Check with Debug configuration! %s", HRESULT_STRINGIZE(result));
	//}

	//D3D11_DEPTH_STENCIL_DESC depthStencilInfo = {};
	//depthStencilInfo.DepthEnable = true;
	//depthStencilInfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilInfo.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	//HRESULT resultA = device->CreateDepthStencilView(depthStencilTex, 0, &depthStencil);
	//if (FAILED(resultA))
	//{
	//	THROW_EXCEPTION("CreateDepthStencilView failed! Check with Debug configuration!");
	//}


	//HRESULT resultB = device->CreateDepthStencilState(&depthStencilInfo, &depthStencilState);
	//if (FAILED(resultB))
	//{
	//	THROW_EXCEPTION("CreateDepthStencilState failed! Check with Debug configuration!");
	//}
}

inline void IDirect3DDevice::createViewport(Vector2<uint32> size)
{
	static Vector2<uint32> cache;
	if (size != 0)
		cache = size;

	D3D11_VIEWPORT vpInfo = {};
	vpInfo.TopLeftX = 0;
	vpInfo.TopLeftY = 0;
	vpInfo.Width = size.x == 0 ? (FLOAT)cache.x : (FLOAT)size.x;
	vpInfo.Height = size.y == 0 ? (FLOAT)cache.y : (FLOAT)size.y;
	vpInfo.MaxDepth = 1.f;
	vpInfo.MinDepth = 0.f;

	deviceContextIMM->RSSetViewports(1, &vpInfo);
}

inline void IDirect3DDevice::createRenderTarget()
{
	MR_ASSERT(swapChain, TEXT("swapChain is invalid!"));

	HRESULT result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(result))
	{
		THROW_EXCEPTION("CreateRenderTargetView failed! Check with Debug configuration!");
	}

	HRESULT resultA = device->CreateRenderTargetView(backBuffer, NULL, &RenderTarget);
	if (FAILED(resultA))
	{
		THROW_EXCEPTION("CreateRenderTargetView failed! Check with Debug configuration!");
	}

	deviceContextIMM->OMSetRenderTargets(1, &RenderTarget, depthStencil);
}


inline void IDirect3DDevice::enumAdapters()
{
	UINT index = 0;
	IDXGIAdapter* tempAdapter;
	while (factory->EnumAdapters(index, &tempAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		availableAdapters.push_back(tempAdapter);
		index++;
	}

	selectedAdapter = 0;
	adapter = availableAdapters[selectedAdapter];
}

inline void IDirect3DDevice::imguiStateChanged(bool NewValue)
{
	//if (NewValue)
	//{
	//	IMGUI_CHECKVERSION();
	//	ImGui::CreateContext();
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.DisplaySize = ImGui::GetMainViewport()->Size;
	//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	//	// Setup Platform/Renderer backends
	//	if (!ImGui_ImplWin32_Init(Application::Get()->getWindowManager()->getFirstWindow()->getWindowHandle()))
	//	{
	//		setImGUIUsed(false);
	//		THROW_EXCEPTION("Failed to Initialize ImGUI (WinAPI implementation)");
	//	}

	//	if (!ImGui_ImplDX11_Init(device, deviceContextIMM))
	//	{
	//		setImGUIUsed(false);
	//		THROW_EXCEPTION("Failed to Initialize ImGUI (DX11 implementation)");
	//	}
	//}
	//else if (getImGUIUsed())
	//{
	//	ImGui_ImplDX11_Shutdown();
	//	ImGui_ImplWin32_Shutdown();
	//	ImGui::DestroyContext();
	//}
}

inline void IDirect3DDevice::createInputLayout()
{

}

inline void IDirect3DDevice::createSampler()
{
	D3D11_SAMPLER_DESC samplerInfo = {};
	samplerInfo.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerInfo.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerInfo.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerInfo.MaxLOD = D3D11_MIP_LOD_BIAS_MAX;
	samplerInfo.MinLOD = 0;
	samplerInfo.MipLODBias = D3D11_MIP_LOD_BIAS_MIN + 5.f;

	HRESULT Result = device->CreateSamplerState(&samplerInfo, &samplerState);
}

inline const String IDirect3DDevice::distinguishFeatureLevel(D3D_FEATURE_LEVEL featureLevel) const
{
	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_1_0_CORE:
		return "1.0 CORE";
	case D3D_FEATURE_LEVEL_9_1:
		return "9.1";
	case D3D_FEATURE_LEVEL_9_2:
		return "9.2";
	case D3D_FEATURE_LEVEL_9_3:
		return "9.3";
	case D3D_FEATURE_LEVEL_10_0:
		return "10.0";
	case D3D_FEATURE_LEVEL_10_1:
		return "10.1";
	case D3D_FEATURE_LEVEL_11_0:
		return "11.0";
	case D3D_FEATURE_LEVEL_11_1:
		return "11.1";
	default:
		return "???";
	}


	return L"???";
}


