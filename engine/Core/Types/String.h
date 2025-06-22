/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Platform/PlatformDefs.h>
#include <string>

/** Human readable piece of text. */
struct String
{
	String() noexcept;

	~String() noexcept;

	String(const char* Input);

	String(const wchar_t* Input);

	String(const int Input);

	String(const float Input);

	String(const unsigned long Input);

	String(const uint32 Input);

	String(const std::string Input);

	String(const std::wstring Input);

	String(const String& other);

	String(String&& other) noexcept;

	String operator=(const String& other);

	String operator+(const String& Other);

	bool operator==(const String& Other) const;
		
	bool operator!=(const String& Other) const;

	bool operator!() const;
	
	String operator+=(const char* other);
	
	const char* operator*();

	operator const char* () { return bIsUsingHeap ? heapBuffer.ptr : stackBuffer.ptr; };

	operator char* ();

	const char* Chr() const;	

	char* Data();

	String Delim(const String character, bool first);

	bool IsEmpty() const noexcept;

	int ToInt() const noexcept;

	float ToFloat() const noexcept;

	uint32 Length() const noexcept;

	static String Format(const String format, ...);

	static bool Contains(const char* buffer, const char* target);
private:
#ifdef MR_DEBUG
	bool bIsInited = false;
#endif // MR_DEBUG

	constexpr void MakeEmpty();

	constexpr void ResetBuffers();

	bool bIsUsingHeap = false;

	union
	{
		struct
		{
			char* ptr = nullptr;

			size_t length = 0;

			size_t capacity = 0;

		} heapBuffer;

		struct
		{ 
			char ptr[sizeof(heapBuffer) - sizeof(unsigned short)] = {'\0'};

			unsigned short length = 0;

		} stackBuffer;
	};

	static constexpr size_t SSO_MAX_CHARS = sizeof(heapBuffer) - sizeof(unsigned short) - 1;
};

String operator+(const String& OtherA, const String& OtherB);
