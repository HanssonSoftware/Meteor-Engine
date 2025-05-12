/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "D3DVertexBuffer.h"
#include <Logging/LogMacros.h>

LOG_ADDCATEGORY(VertexBuffer);

void D3DVertexBuffer::Init()
{
	if (buffer == nullptr)
	{
		MR_LOG(LogVertexBuffer, Error, "Unable to create buffer! Output device is invalid!");
		return;
	}
}

void* D3DVertexBuffer::getData() const
{
	return buffer;
}
