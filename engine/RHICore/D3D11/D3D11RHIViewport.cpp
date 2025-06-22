/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "D3D11RHIViewport.h"

D3D11RHIViewport::D3D11RHIViewport(IRHIRegistry* owningRegistry, const uint32 sizeX, const uint32 sizeY, const uint32 posX, const uint32 posY)
	: IRHIViewport(owningRegistry, sizeX, sizeY, posX, posY)
{

}

void D3D11RHIViewport::Init()
{
	IRHIViewport::Init();
}

void D3D11RHIViewport::CleanUp()
{
	IRHIViewport::CleanUp();
}
