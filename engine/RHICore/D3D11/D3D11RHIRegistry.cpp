/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "D3D11RHIRegistry.h"

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
	return true;
}

void D3D11RHIRegistry::CleanUp() const
{
}
