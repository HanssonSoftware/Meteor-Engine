/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Log/LogMacros.h>
#include <vector>
#include <string>
#include "PlatformDefs.h"


/** Human readable piece of text. */
class String
{
public:
	String();

	String(const char* Input);

	String(const wchar_t* Input);

	String(int Input);

	String(float Input);

	String(uint32 Input);

	String(const std::string Input);

	String(const std::wstring Input);

	~String();

	String operator+(const String& Other);
	
	bool operator==(const String& Other);
	
	/** Chr() is the alternative. */
	const wchar_t* operator*();

	/** (*this) is the alternative. */
	const wchar_t* Chr();

	const wchar_t* Chr() const;

	void upper();

	bool isEmpty() const;

private:
	std::vector<wchar_t> buffer;
};
