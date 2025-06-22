/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <RHI/RHIOutputContext.h>


class D3D11OutputContext : public IRHIOutputContext
{
	friend class D3D11RHIRegistry;
public:
	D3D11OutputContext(IRHIRegistry* hive) : IRHIOutputContext(hive)
	{

	}

	virtual void Clear() override;

	virtual void Draw() override;

	virtual void CleanUp() const override;

	virtual void CompileShader(const String name) override;
};

