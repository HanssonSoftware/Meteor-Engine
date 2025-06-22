/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <RHI/RHIRegistry.h>

struct IDXGIFactory;
struct IDXGIAdapter;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;

class D3D11RHIRegistry : public IRHIRegistry
{
	friend class D3D11OutputContext;
public:
	virtual int GetMonitorIndex() const override;

	virtual String GetName() const override;

	virtual bool Initialize() override;

	virtual void CleanUp() const override;

protected:
	std::vector<IDXGIAdapter*> adapters;

	IDXGIFactory* factory;

	ID3D11Device* device;

	ID3D11DeviceContext* deviceContext;

	IDXGISwapChain* swapChain;

	ID3D11Buffer* backBuffer;

	ID3D11RenderTargetView* rtv;
};

