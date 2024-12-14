/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "String.h"
#include <Common/MemoryManager.h>
#include <Log/Exception.h>


String::String()
{
	buffer = (wchar_t*)mrmalloc(sizeof(wchar_t));
}

/**
* So this fuck (mbstowcs), doesn't include null terminator count at the return.
*/
String::String(const char* Input)
{
	if (!Input)
	{
		THROW_EXCEPTION("Invalid or NULL buffer!");
	}

	/** Recommended size, only char*!*/
	const size_t bufferRecommendedSize = mbstowcs(NULL, Input, 0);

	buffer = (wchar_t*)mrmalloc((bufferRecommendedSize + 1) * sizeof(wchar_t));

	mbstowcs(buffer, Input, bufferRecommendedSize + 1);
}


String::String(wchar_t* buffer)
{
	if (!buffer)
	{
		THROW_EXCEPTION("Invalid or NULL buffer!");
	}

	const size_t requiredSize = wcslen(buffer);

	this->buffer = (wchar_t*)mrmalloc((requiredSize + 1) * sizeof(wchar_t));

	wmemcpy(this->buffer, buffer, requiredSize + 1);
}


String::String(const wchar_t* Input)
{
	if (!Input)
	{
		THROW_EXCEPTION("Invalid or NULL buffer!");
	}

	const size_t length = wcslen(Input) + 1;

	buffer = (wchar_t*)mrmalloc(length * sizeof(wchar_t));

	wmemcpy(buffer, Input, length);
}

String::String(int Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer = (wchar_t*)mrmalloc((wcslen(convertedString.c_str()) + 1) * sizeof(wchar_t));

	wmemcpy(buffer, convertedString.c_str(), wcslen(convertedString.c_str()) + 1);
}

String::String(uint32 Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer = (wchar_t*)mrmalloc((wcslen(convertedString.c_str()) + 1) * sizeof(wchar_t));

	wmemcpy(buffer, convertedString.c_str(), wcslen(convertedString.c_str()) + 1);
}

String::String(float Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer = (wchar_t*)mrmalloc((wcslen(convertedString.c_str()) + 1) * sizeof(wchar_t));

	wmemcpy(buffer, convertedString.c_str(), wcslen(convertedString.c_str()) + 1);
}

String::String(const std::string Input)
{
	const size_t bufferRecommendedSize = mbstowcs(NULL, Input.c_str(), 0);
	buffer = (wchar_t*)mrmalloc((bufferRecommendedSize + 1) * sizeof(wchar_t));

	mbstowcs(buffer, Input.c_str(), bufferRecommendedSize + 1);
}

String::String(const std::wstring Input)
{
	const size_t recommendedSize = wcslen(Input.c_str()) + 1;
	buffer = (wchar_t*)mrmalloc(recommendedSize * sizeof(wchar_t));

	wmemcpy(buffer, Input.c_str(), recommendedSize);
}

String::String(String&& other) noexcept
{
	const size_t recommendedSize = wcslen(other.buffer) + 1;
	buffer = (wchar_t*)mrmalloc(recommendedSize * sizeof(wchar_t));

	wmemcpy(buffer, other.buffer, recommendedSize);
}

String::String(const String& other)
{
	const size_t recommendedSize = wcslen(other.buffer) + 1;
	buffer = (wchar_t*)mrmalloc(recommendedSize * sizeof(wchar_t));

	wmemcpy(buffer, other.buffer, recommendedSize);
}

String::~String()
{
	if (buffer != nullptr)
	{
		mrfree(buffer);
		buffer = nullptr;
	}
}

String String::operator+(const String& Other)
{
	// Querry the buffers.
	const wchar_t* otherBuffer = Other.buffer;
	const wchar_t* thisBuffer = buffer;

	// Copy paste from https://en.cppreference.com/w/c/string/wide/wcslen.
	// So I found out that wcslen() does not count in the \0.
	const size_t size = wcslen(thisBuffer) + wcslen(otherBuffer) + 1;

	wchar_t* super = (wchar_t*)mrmalloc(size * sizeof(wchar_t));

	if (super == nullptr)
	{
		THROW_EXCEPTION("String concencation failed! Wide buffer is null!");
	}

	wcscpy(super, thisBuffer);
	wcscat(super, otherBuffer);

	String newStringBuffer(super);

	mrfree(super);
	return newStringBuffer;
}

bool String::operator==(const String& Other) const
{
	return wcscmp(buffer, Other.buffer) == 0;
}

const wchar_t* String::operator*()
{
	return buffer ? buffer : L"";
}

const wchar_t* String::Chr()
{
	return buffer ? buffer : L"";
}

void String::Narrow(const wchar_t* wideString, char*& narrowString)
{
	const size_t narrowSize = wcstombs(NULL, wideString, 0);

	char* narrowBuffer = (char*)mrmalloc((narrowSize + 1) * sizeof(char));

	if (narrowBuffer == nullptr)
		THROW_EXCEPTION("Unable to convert from Wide to Narrow! Buffer is null!");

	const size_t converted = wcstombs(narrowBuffer, wideString, narrowSize);

	if (converted == 0)
		THROW_EXCEPTION("No characters converted!");

	strcpy(narrowString, narrowBuffer);
	mrfree(narrowBuffer);
}

const wchar_t* String::Chr() const
{ 
	return buffer ? buffer : L"";
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
	return buffer == nullptr || wcslen(buffer) == 0;
}

int String::toInt() const
{
	wchar_t* end;
	return wcstol(buffer, &end, 10);
}

float String::toFloat() const
{
	wchar_t* end;
	return wcstof(buffer, &end);
}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		if (buffer != nullptr)
			mrfree(buffer);

		const size_t otherBuffSize = wcslen(other.buffer) + 1;
		buffer = (wchar_t*)mrmalloc(otherBuffSize * sizeof(wchar_t));
		wmemcpy(buffer, other.buffer, otherBuffSize);
	}

	return *this;
}


//String::String()
//{
//	buffer = L'\0';
//}
//
///**
//* So this fuck (mbstowcs), doesn't include null terminator count at the return.
//*/
//String::String(const char* Input)
//{
//	/** Recommended size, only char*!*/
//	const size_t bufferRecommendedSize = mbstowcs(NULL, Input, 0);
//
//	wchar_t* bufferMemory = (wchar_t*)mrmalloc((bufferRecommendedSize + 1) * sizeof(wchar_t));
//
//	mbstowcs(bufferMemory, Input, strlen(Input) + 1);
//
//	(bufferMemory, bufferMemory + wcslen(bufferMemory) + 1);
//
//	mrfree(bufferMemory);
//}
//
//String::String(const wchar_t* Input)
//{
//	buffer.assign(Input, Input + wcslen(Input));
//	buffer.push_back(L'\0');
//}
//
//String::String(int Input)
//{
//	const std::wstring convertedString = std::to_wstring(Input);
//	buffer.assign(convertedString.c_str(), convertedString.c_str() + wcslen(convertedString.c_str()) + 1);
//}
//
//String::String(uint32 Input)
//{
//	const std::wstring convertedString = std::to_wstring(Input);
//	buffer.assign(convertedString.c_str(), convertedString.c_str() + wcslen(convertedString.c_str()) + 1);
//}
//
//String::String(float Input)
//{
//	const std::wstring convertedString = std::to_wstring(Input);
//	buffer.assign(convertedString.c_str(), convertedString.c_str() + wcslen(convertedString.c_str()) + 1);
//}
//
//String::String(const std::string Input)
//{
//	const size_t bufferRecommendedSize = mbstowcs(NULL, Input.c_str(), 0);
//	wchar_t* bufferMemory = (wchar_t*)mrmalloc((bufferRecommendedSize + 1) * sizeof(wchar_t));
//
//	mbstowcs(bufferMemory, Input.c_str(), bufferRecommendedSize);
//	buffer.assign(bufferMemory, bufferMemory + bufferRecommendedSize);
//	buffer.push_back(L'\0');
//
//	mrfree(bufferMemory);
//}
//
//String::String(const std::wstring Input)
//{
//	buffer.assign(Input.c_str(), Input.c_str());
//	buffer.push_back(L'\0');
//}
////
////String::~String()
////{
////#pragma warning(disable : 6031)
////	buffer.clear();
////}
//
//String String::operator+(const String& Other)
//{
//	// Querry the buffers.
//	const wchar_t* otherBuffer = Other.buffer.data();
//	const wchar_t* thisBuffer = buffer.data();
//
//	// Copy paste from https://en.cppreference.com/w/c/string/wide/wcslen.
//	// So I found out that wcslen() does not count in the \0.
//	const size_t size = wcslen(thisBuffer) + wcslen(otherBuffer) + 1;
//
//	wchar_t* super = (wchar_t*)mrmalloc(size * sizeof(wchar_t));
//
//	if (super == nullptr)
//		throw Exception("String concencation failed! Wide buffer is null!");
//
//	wcscpy(super, thisBuffer);
//	wcscat(super, otherBuffer);
//
//	String newStringBuffer(super);
//
//	mrfree(super);
//	return newStringBuffer;
//}
//
//bool String::operator==(const String& Other) const
//{
//	return wcscmp(buffer.data(), Other.buffer.data()) == 0;
//}
//
//const wchar_t* String::operator*()
//{
//	return buffer.data();
//}
//
//const wchar_t* String::Chr()
//{
//	return buffer.data();
//}
//
//void String::Narrow(const wchar_t* wideString, char*& narrowString)
//{
//	const size_t narrowSize = wcstombs(NULL, wideString, 0);
//
//	char* narrowBuffer = (char*)mrmalloc((narrowSize + 1) * sizeof(char));
//
//	if (narrowBuffer == nullptr)
//		throw Exception("Unable to convert from Wide to Narrow! Buffer is null!");
//
//	const size_t converted = wcstombs(narrowBuffer, wideString, narrowSize);
//
//	if (converted == 0)
//		throw Exception("No characters converted!");
//
//	strcpy(narrowString, narrowBuffer);
//	mrfree(narrowBuffer);
//}
//
//const wchar_t* String::Chr() const
//{
//	return buffer.data();
//}
//
//void String::upper()
//{
//	//const wchar_t* charbuffer = buffer.data();
//	//const size_t size = wcslen(charbuffer); // We don't need \0
//
//	//for (wchar_t& indexed : buffer)
//	//{
//	//	towupper(indexed);
//	//}
//}
//
//bool String::isEmpty() const
//{
//	if (buffer.size() <= 1)
//		return true;
//
//	return false;
//}
