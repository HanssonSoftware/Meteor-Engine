/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "D3D11RHIRegistry.h"
#include "D3D11OutputContext.h"
#include <Logging/LogMacros.h>
#include <Windows/WindowsLayer.h>
#include <Platform/WindowManager/WindowManager.h>

#include <d3d11.h>
#include <dxgi.h>
#include <Application.h>
#include <Commandlet.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

LOG_ADDCATEGORY(D3D11);

#define D3D_CHECK(result, funcname) \
	if (FAILED(result)) \
	{ \
		if (WindowsLayer* winLayer = (WindowsLayer*)Layer::GetSystemLayer()) \
		{ \
			MR_LOG(LogD3D11, Error, "%s returned with: %s.", #funcname, winLayer->GetError(result).Chr()); \
			return false; \
		} \
		else \
		{ \
			MR_LOG(LogD3D11, Error, "%s returned with: Unknown error!", #funcname); \
			return false; \
		} \
	}

#define CLEANIFVALID(prop)\
	if (prop != nullptr)\
	{ \
		prop->Release();\
		/*prop = nullptr;*/ \
	}


int D3D11RHIRegistry::GetMonitorIndex() const
{
	return 0;
}

static String privRHIName;
String D3D11RHIRegistry::GetName() const
{
	static constexpr const wchar_t* vkRHIDisplayName = L"D3D11";

	if (privRHIName.IsEmpty()) privRHIName = vkRHIDisplayName;

	return privRHIName;
}

bool D3D11RHIRegistry::Initialize()
{
	MR_LOG(LogD3D11, Log, "Initializing Direct3D!");

	context = new D3D11OutputContext(this);

	{
		const HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		D3D_CHECK(result, CreateDXGIFactory);
	}

	{
		const HRESULT result = factory->MakeWindowAssociation((HWND)App::Get()->GetFirstWindow()->GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER);
		D3D_CHECK(result, MakeWindowAssociation);
	}

	int gpuIndex = 0;
	IDXGIAdapter* currentAdapter;
	while (factory->EnumAdapters(gpuIndex, &currentAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.push_back(currentAdapter);
		gpuIndex++;
	}

	{
		const int gpuID = !ICommandlet::Parse("-gpuid").IsEmpty() ? ICommandlet::Parse(nullptr).ToInt() : 0;

#ifdef MR_DEBUG
		constexpr const unsigned int cdFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
		constexpr const unsigned int cdFlags = 0;
#endif // MR_DEBUG

		constexpr const D3D_FEATURE_LEVEL availableFeatureLevels = {
				D3D_FEATURE_LEVEL_11_0
		};

		D3D_FEATURE_LEVEL selectedFeatureLevel;

		HRESULT res = D3D11CreateDevice(
			adapters[gpuID],
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			cdFlags,
			&availableFeatureLevels,
			1,
			D3D11_SDK_VERSION,
			&device,
			&selectedFeatureLevel,
			&deviceContext);

		D3D_CHECK(res, D3D11CreateDevice);
	}

	{
		IWindow* win = App::Get()->GetWindowManager()->GetFirstWindow();

		DXGI_SWAP_CHAIN_DESC swapChainCreationInfo = {};
		swapChainCreationInfo.BufferDesc.Width = win->GetSize().x;
		swapChainCreationInfo.BufferDesc.Height = win->GetSize().y;
		swapChainCreationInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		swapChainCreationInfo.Windowed = 1;

		swapChainCreationInfo.SampleDesc.Count = 1;
		swapChainCreationInfo.SampleDesc.Quality = 0;

		swapChainCreationInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainCreationInfo.BufferCount = 2;
		swapChainCreationInfo.BufferDesc.RefreshRate.Denominator = 165;
		swapChainCreationInfo.BufferDesc.RefreshRate.Numerator = 1;
		swapChainCreationInfo.OutputWindow = (HWND)win->GetWindowHandle();
		swapChainCreationInfo.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainCreationInfo.Flags = 0;

		HRESULT res = factory->CreateSwapChain(device, &swapChainCreationInfo, &swapChain);
		D3D_CHECK(res, IDXGIFactory::CreateSwapChain);
	}

	{

		HRESULT result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		D3D_CHECK(result, IDXGISwapChain::GetBuffer);

		HRESULT res = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);
		D3D_CHECK(res, ID3D11Device::CreateRenderTargetView);
	}

	{

		deviceContext->OMSetRenderTargets(1, &rtv, 0);

		IWindow* win = App::Get()->GetWindowManager()->GetFirstWindow();
		
		D3D11_VIEWPORT vp = {};
		vp.Width = (float)win->GetSize().x;
		vp.Height = (float)win->GetSize().y;

		deviceContext->RSSetViewports(1, &vp);
	}

	return true;
}

void D3D11RHIRegistry::CleanUp() const
{
	CLEANIFVALID(swapChain);

	CLEANIFVALID(factory);

	CLEANIFVALID(deviceContext);
	
	CLEANIFVALID(device);
}
