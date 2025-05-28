/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "PlatformDefs.h"
#include <string>

/** Human readable piece of text. */
struct String
{
	String() noexcept;

	String(const char* Input);

	String(const wchar_t* Input);

	String(const int Input);

	String(const float Input);

	String(const unsigned long Input);

	String(const uint32 Input);

	String(const std::string Input);

	String(const std::wstring Input);

	~String();

	String(wchar_t* buffer);

	String(const String& other);

	String(String&& other) noexcept;

	String& operator=(const String& other);

	String operator+(const String& Other);

	bool operator==(const String& Other) const;
		
	bool operator!=(const String& Other) const;

	bool operator!=(String& Other) const;

	char* operator=(const String& other) const
	{
		return other.buffer;
	}
	
	/** Chr() is the alternative. */
	const char* operator*();

	operator const char*()
	{
		return buffer;
	}

	operator char*()
	{
		return buffer;
	}

	/** (*this) is the alternative. */
	const char* Chr();	

	char* Data();

	const char* Chr() const;

	String Delim(const String character, bool first);

	bool IsEmpty() const;

	bool EndsWith(const String string) const;

	int ToInt() const;

	float ToFloat() const;

	uint32 Length() const;

	static String Format(const String format, ...);
private:
#ifdef MR_DEBUG
	bool bIsInited = false;
#endif // MR_DEBUG

	char* buffer = nullptr;
	//static constexpr const int MAX_STRING_SIZE = 16;

	//static constexpr bool USE_HEAP = false;
	//
	//union stb
	//{
	//	char* finite[MAX_STRING_SIZE + 1];

	//	char* infinite = nullptr;

	//} buffer;
};

String operator+(const String& OtherA, const String& OtherB);
