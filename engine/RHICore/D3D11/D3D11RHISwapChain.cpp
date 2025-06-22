/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "D3D11RHISwapChain.h"
#include <d3d11.h>

void D3D11RHISwapChain::Present()
{
	HRESULT result = object->Present(bIsUsingVsync ? 1 : 0, 0);
}
