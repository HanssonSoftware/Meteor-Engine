/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <RHI/VertexBuffer.h>

class ID3D11VertexBuffer;

class D3DVertexBuffer : public VertexBuffer
{
public:
	virtual void Init() override;

	virtual void* getData() const;

private:
	ID3D11VertexBuffer* buffer;
};

