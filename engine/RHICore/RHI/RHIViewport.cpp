/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "RHIViewport.h"

IRHIViewport::IRHIViewport(IRHIRegistry* owningRegistry, const uint32 sizeX, const uint32 sizeY, const uint32 posX, const uint32 posY)
	: registry(owningRegistry)
	, sizeX(sizeX)
	, sizeY(sizeY)
	, posX(posX)
	, posY(posY)
{

}
