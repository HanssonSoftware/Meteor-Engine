/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <RHI/RHICommons.h>

class IRHIRegistry;

/** WARNING: Before doing rendering, make sure this is called!*/
class RHILoader
{
public:
	static IRHIRegistry* InitRHI(const SystemLayout layout);
private:
	static IRHIRegistry* InitWindows() noexcept;
};

