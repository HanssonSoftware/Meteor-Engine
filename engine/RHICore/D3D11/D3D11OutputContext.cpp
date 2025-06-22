/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "D3D11OutputContext.h"
#include "D3D11RHIRegistry.h"
#include <Types/String.h>
#include <Logging/LogMacros.h>
#include <Common/Math.h>

#include <d3d11.h>

LOG_ADDCATEGORY(D3D11);

void D3D11OutputContext::Clear()
{
	MR_ASSERT(registry != nullptr, "Invalid D3D11OutputContext");

	D3D11RHIRegistry* registryA = (D3D11RHIRegistry*)registry;

	float j = 0.45f;

	float a = Math::EaseInOut<float>(j);
	static const float clearColor[4] = { a, a, 1.f, 1.f};
	registryA->deviceContext->ClearRenderTargetView(registryA->rtv, clearColor);
}

void D3D11OutputContext::Draw()
{
	MR_ASSERT(registry != nullptr, "Invalid D3D11OutputContext");

	D3D11RHIRegistry* registryA = (D3D11RHIRegistry*)registry;

	registryA->swapChain->Present(0, 0);
}

void D3D11OutputContext::CleanUp() const
{
}


void D3D11OutputContext::CompileShader(const String name)
{
}
