/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once


class IRHIBuffer
{
public:
	virtual void SetData(void* newInput) = 0;

	virtual void* GetData() const = 0;

	virtual ~IRHIBuffer() = default;
protected:
	/* (void* data) must be declared at the children!*/
};

