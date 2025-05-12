/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once

class IRHIRegistry;

class IRHIShader
{
public:
	IRHIShader(IRHIRegistry* Registry);

	virtual ~IRHIShader() = default;

	virtual void* GetData() = 0;
protected:
	IRHIRegistry* registry;
};

