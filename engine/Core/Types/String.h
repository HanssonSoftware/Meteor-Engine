/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>

/** Human readable piece of text. */
class String
{
public:
	String() noexcept;

	~String() noexcept;

	String(const char* Input);

	String(const wchar_t* Input);

	String(const int32_t Input);

	String(const float Input);

	String(const unsigned long Input);

	String(const uint32_t Input);

	String(const String& other);

	String(const char* string, uint32_t length);

	String(String&& other) noexcept;

	String operator=(const String& other);

	String operator+(const String& Other);

	bool operator==(const String& Other) const;
		
	bool operator!=(const String& Other) const;

	bool operator!() const;
	
	String operator+=(const String& other);
	
	const char* operator*() { return bIsUsingHeap ? heapBuffer.ptr : stackBuffer.ptr; };

	operator const char* () const { return bIsUsingHeap ? heapBuffer.ptr : stackBuffer.ptr; };

	const char* Chr() const;	

	/** Creates an allocated string, straight from this container. Do not forget to delete! */
	char* Allocate();	

	String Delim(const String character, bool first);

	bool IsEmpty() const noexcept;

	int32_t ToInt() const noexcept;

	float ToFloat() const noexcept;

	uint32_t Length() const noexcept;
	
	/** */
	static String Format(const String format, ...);

	static bool Contains(const char* buffer, const char* target);
	
	static bool IsWhitespace(const char* buffer);

	static bool IsSpace(const char* buffer);

	/** Checks whenever the passed parameter is a letter. Upper/Lower case.*/
	static bool IsAlpha(const char* buffer);

private:
	constexpr void MakeEmpty();

	constexpr void ResetBuffers();

	bool bIsUsingHeap = false;

	union
	{
		struct
		{
			char* ptr = nullptr;

			uint32_t length = 0;

			uint32_t capacity = 0;

		} heapBuffer;

		struct
		{ 
			char ptr[sizeof(heapBuffer) - sizeof(uint16_t)] = {'\0'};

			uint16_t length = 0;

		} stackBuffer;
	};

	static constexpr uint16_t SSO_MAX_CHARS = sizeof(heapBuffer) - sizeof(uint16_t) - 1;

#ifdef MR_DEBUG
	bool bIsInited = false;
#endif // MR_DEBUG
};

String operator+(const String& OtherA, const String& OtherB);
