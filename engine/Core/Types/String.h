/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "PlatformDefs.h"
#include <string>

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

	String(wchar_t* buffer);

	String(const String& other);

	String(String&& other) noexcept;

	String& operator=(const String& other);

	String operator+(const String& Other);
	
	bool operator==(const String& Other) const;
	
	/** Chr() is the alternative. */
	const wchar_t* operator*();

	/** (*this) is the alternative. */
	const wchar_t* Chr();

	static void Narrow(const wchar_t* wideString, char*& narrowString);

	const wchar_t* Chr() const;

	void upper();

	bool isEmpty() const;

	int toInt() const;

	float toFloat() const;

private:
	wchar_t* buffer;
public:
};
