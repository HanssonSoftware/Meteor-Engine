/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once

class IRHIRegistry;
struct String;

class IRHIOutputContext
{
public:
	IRHIOutputContext(IRHIRegistry* hive) 
		: registry(hive)
	{
	
	};

	virtual ~IRHIOutputContext() = default;

	virtual void Clear() = 0;

	virtual void Draw() = 0;

	virtual void CleanUp() const = 0;

	virtual void CompileShader(const String name) = 0;

	IRHIRegistry* GetRegistry() const { return registry; };

protected:
	IRHIRegistry* registry;
};

