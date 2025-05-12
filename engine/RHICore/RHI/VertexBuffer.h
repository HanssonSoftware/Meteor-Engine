/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */


#pragma once

class IRHIOutputContext;

/**
*	
*/
class VertexBuffer
{
public:
	virtual void Init() = 0;

	virtual void Destroy() = 0;

	virtual void* getData() const = 0;

	virtual ~VertexBuffer() = default;

protected:
	IRHIOutputContext* ctx;
};

