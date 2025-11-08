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

	String& operator=(const String& other);

	String operator+(const String& Other);

	bool operator==(const String& Other) const;

	bool operator==(const char* Other) const;
		
	bool operator!=(const String& Other) const;

	bool operator!() const
	{
		return bIsUsingHeap ? !heapBuffer.ptr : !stackBuffer.ptr;
	}

	operator bool() const
	{
		return Length() > 0;
	}

	String& operator+=(const String& other);
	
	const char* operator*() const
	{
		return bIsUsingHeap ? (heapBuffer.ptr || heapBuffer.length != 0 ? heapBuffer.ptr : "") :
			stackBuffer.ptr || stackBuffer.length != 0 ? stackBuffer.ptr : "";
	}

	operator const char* () const;

	const char* Chr() const
	{
		return bIsUsingHeap ? (heapBuffer.ptr || heapBuffer.length != 0 ? heapBuffer.ptr : "") :
			stackBuffer.ptr || stackBuffer.length != 0 ? stackBuffer.ptr : "";
	}

	/** Creates an allocated string, straight from this container. Do not forget to delete! */
	char* Allocate();	

	String Delim(const String character, bool first);

	bool IsEmpty() const noexcept;

	int32_t ToInt() const noexcept;

	float ToFloat() const noexcept;

	uint32_t Length() const noexcept
	{
		return bIsUsingHeap ? (uint32_t)heapBuffer.length : (uint32_t)stackBuffer.length;
	}
	
	/** */
	static String Format(const String& format, ...);

	static bool Contains(const char* buffer, const char* target);
	
	static bool IsWhitespace(const char* buffer);

	static bool IsSpace(const char* buffer);

	/** Checks whenever the passed parameter is a letter. Upper/Lower case.*/
	static bool IsAlpha(const char* buffer);

private:
	void MakeEmpty();

	void ResetBuffers();

	char* Data() { return bIsUsingHeap ? heapBuffer.ptr : stackBuffer.ptr; };

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

	bool bIsUsingHeap = false;

#ifdef MR_DEBUG
	bool bIsInited = false;
#endif // MR_DEBUG
};

String operator+(const String& OtherA, const String& OtherB);
