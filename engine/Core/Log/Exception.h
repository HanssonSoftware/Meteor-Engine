/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>

class Exception
{

public:

	Exception(String text);

	Exception(String text, int line, const wchar_t* File, const wchar_t* function, ...);

	//Exception(String text, int line, const wchar_t* function);

protected:
	String description;

public:
	String What() const { return description; };

	bool operator==(const Exception& other) const;
};

#define THROW_EXCEPTION(message, ...) throw Exception(message, __LINE__, __FILEW__ ,__FUNCTIONW__, __VA_ARGS__);