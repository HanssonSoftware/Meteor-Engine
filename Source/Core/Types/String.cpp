/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#include "String.h"
//#include <wctype.h>
#include <Common/MemoryManager.h>


String::String()
{
	buffer.push_back(L'\0');
}

/**
* So this fuck (mbstowcs), doesn't include null terminator count at the return.
*/
String::String(const char* Input)
{
	/** Recommended size, only char*!*/
	const size_t bufferRecommendedSize = mbstowcs(NULL, Input, 0);

	wchar_t* bufferMemory = (wchar_t*)mrmalloc((bufferRecommendedSize + 1) * sizeof(wchar_t));

	mbstowcs(bufferMemory, Input, strlen(Input) + 1);

	buffer.assign(bufferMemory, bufferMemory + wcslen(bufferMemory) + 1);

	mrfree(bufferMemory);
}

String::String(const wchar_t* Input)
{
	buffer.assign(Input, Input + wcslen(Input));
	buffer.push_back(L'\0');
}

String::String(int Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer.assign(convertedString.c_str(), convertedString.c_str() + wcslen(convertedString.c_str()) + 1);
}

String::String(uint32 Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer.assign(convertedString.c_str(), convertedString.c_str() + wcslen(convertedString.c_str()) + 1);
}

String::String(float Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer.assign(convertedString.c_str(), convertedString.c_str() + wcslen(convertedString.c_str()) + 1);
}

String::String(const std::string Input)
{
	const size_t bufferRecommendedSize = mbstowcs(NULL, Input.c_str(), 0);
	wchar_t* bufferMemory = (wchar_t*)mrmalloc((bufferRecommendedSize + 1) * sizeof(wchar_t));

	mbstowcs(bufferMemory, Input.c_str(), bufferRecommendedSize);
	buffer.assign(bufferMemory, bufferMemory + bufferRecommendedSize);
	buffer.push_back(L'\0');

	mrfree(bufferMemory);
}

String::String(const std::wstring Input)
{
	buffer.assign(Input.c_str(), Input.c_str());
	buffer.push_back(L'\0');
}

String::~String()
{
#pragma warning(disable : 6031)
	buffer.clear();
}

String String::operator+(const String& Other)
{
	// Querry the buffers.
	const wchar_t* otherBuffer = Other.buffer.data();
	const wchar_t* thisBuffer = buffer.data();

	// Copy paste from https://en.cppreference.com/w/c/string/wide/wcslen.
	// So I found out that wcslen() does not count in the \0.
	const size_t size = wcslen(thisBuffer) + wcslen(otherBuffer) + 1;

	wchar_t* super = (wchar_t*)mrmalloc(size * sizeof(wchar_t));

	wcscpy(super, thisBuffer);
	wcscat(super, otherBuffer);

	String newStringBuffer(super);

	mrfree(super);
	return newStringBuffer;
}

bool String::operator==(const String& Other)
{
	return wcscmp(buffer.data(), Other.buffer.data()) == 0;
}

const wchar_t* String::operator*()
{
	return buffer.data();
}

const wchar_t* String::Chr()
{
	return buffer.data();
}

const wchar_t* String::Chr() const
{ 
	return buffer.data();
}

void String::upper()
{
	//const wchar_t* charbuffer = buffer.data();
	//const size_t size = wcslen(charbuffer); // We don't need \0

	//for (wchar_t& indexed : buffer)
	//{
	//	towupper(indexed);
	//}
}

bool String::isEmpty() const
{
	if (buffer.size() <= 1)
		return true;

	return false;
}
