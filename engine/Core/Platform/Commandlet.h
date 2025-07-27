/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Logging/LogMacros.h>
//#include <Types/String.h>

struct String;

LOG_ADDCATEGORY(Commandlet);

class ICommandlet
{
public:
	static void Initalize();

	virtual ~ICommandlet() noexcept = default;

	static String Parse(const char* inParam);

	static String Parse(const char* buffer, const char* inParam);
};