/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "PlatformDefs.h"
#include <string>

/** Human readable piece of text. */
class String
{
public:
	String() noexcept;

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
		
	bool operator!=(const String& Other) const;

	bool operator!=(String& Other) const;
	
	/** Chr() is the alternative. */
	const wchar_t* operator*();

	/** (*this) is the alternative. */
	const wchar_t* Chr();	

	wchar_t* Data();

	static void Narrow(const wchar_t* wideString, char*& narrowString);

	const wchar_t* Chr() const;

	void upper();

	String Delim(const String character, bool first);

	bool isEmpty() const;

	bool endsWith(const String string) const;

	int toInt() const;

	float toFloat() const;

	uint32 Length() const;

	static String Format(const String format, ...);

	/** Iterates from the given index until the next '\n', updating the index to the '\n' position. */
	static inline String readLine(const String Line, uint32 location);

	/** Iterates from the given index until the next '\n', updating the index to the '\n' position. */
	static String readLine(const wchar_t* Line, uint32 location);
private:
#ifdef MR_DEBUG
	bool bIsInited = false;
#endif // MR_DEBUG
	wchar_t* buffer = nullptr;
};

String operator+(const String& OtherA, const String& OtherB);
