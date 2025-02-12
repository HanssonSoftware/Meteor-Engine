/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "String.h"
#include <Common/MemoryManager.h>
#include <Log/Exception.h>
#include <Common/Pointers.h>


String::String() noexcept
{
	
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
	if (bufferRecommendedSize == 0)
		return;

	buffer = new wchar_t[(bufferRecommendedSize + 1) * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (mbstowcs(buffer, Input, bufferRecommendedSize + 1) != bufferRecommendedSize)
	{
		delete[] buffer;
		THROW_EXCEPTION("Failed to Convert Narrow String to Wide!");
	}
}


String::String(wchar_t* buffer)
{
	if (!buffer)
	{
		THROW_EXCEPTION("Invalid or NULL buffer!");
	}

	const size_t requiredSize = wcslen(buffer);
	if (requiredSize == 0)
		return;

	this->buffer = new wchar_t[(requiredSize + 1) * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (wcscmp(wcsncpy(this->buffer, buffer, requiredSize + 1), buffer) != 0)
	{
		delete[] this->buffer;
		THROW_EXCEPTION("Failed to Convert Copy Widestrings!");
	}
}


String::String(const wchar_t* Input)
{
	if (!Input)
	{
		THROW_EXCEPTION("Invalid or NULL buffer!");
	}

	const size_t length = wcslen(Input) + 1;
	if (length == 0)
		return;

	buffer = new wchar_t[length * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (wcscmp(wcsncpy(buffer, Input, length), Input) != 0)
	{
		delete[] buffer;
		THROW_EXCEPTION("Failed to Copy Widestrings!");
	}
}

String::String(int Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer = new wchar_t[(wcslen(convertedString.c_str()) + 1) * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	wcsncpy(buffer, convertedString.c_str(), wcslen(convertedString.c_str()) + 1);
}

String::String(uint32 Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer = new wchar_t[(wcslen(convertedString.c_str()) + 1) * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	wcsncpy(buffer, convertedString.c_str(), wcslen(convertedString.c_str()) + 1);
}

String::String(float Input)
{
	const std::wstring convertedString = std::to_wstring(Input);
	buffer = new wchar_t[(wcslen(convertedString.c_str()) + 1) * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	wcsncpy(buffer, convertedString.c_str(), wcslen(convertedString.c_str()) + 1);
}

String::String(const std::string Input)
{
	const size_t bufferRecommendedSize = mbstowcs(NULL, Input.c_str(), 0);
	buffer = new wchar_t[(bufferRecommendedSize + 1) * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	mbstowcs(buffer, Input.c_str(), bufferRecommendedSize + 1);
}

String::String(const std::wstring Input)
{
	const size_t recommendedSize = wcslen(Input.c_str()) + 1;
	buffer = new wchar_t[recommendedSize * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	wmemcpy(buffer, Input.c_str(), recommendedSize);
}

String::String(String&& other) noexcept
{
	if (other.buffer == nullptr)
		return;

	const size_t recommendedSize = wcslen(other.buffer) + 1;
	buffer = new wchar_t[recommendedSize * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	wcsncpy(buffer, other.buffer, recommendedSize);
}

String::String(const String& other)
{
	const size_t recommendedSize = wcslen(other.buffer) + 1;
	buffer = new wchar_t[recommendedSize * sizeof(wchar_t)]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	wcsncpy(buffer, other.buffer, recommendedSize);
}

String::~String()
{
	if (buffer != nullptr)
	{
		delete[] buffer;

		buffer = nullptr;
#ifdef MR_DEBUG
		bIsInited = false;
#endif // MR_DEBUG
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

	wchar_t* super = new wchar_t[size * sizeof(wchar_t)]();

	if (super == nullptr)
	{
		delete[] super;
		THROW_EXCEPTION("String concencation failed! Wide buffer is null!");
	}

	wcscpy(super, thisBuffer);
	wcscat(super, otherBuffer);

	String newStringBuffer(super);

	delete[] super;
	return newStringBuffer;
}


String operator+(const String& OtherA, const String& OtherB)
{
	// Querry the buffers.
	const wchar_t* otherABuffer = OtherA.Chr();
	const wchar_t* otherBBuffer = OtherB.Chr();

	const size_t size = wcslen(otherABuffer) + wcslen(otherBBuffer) + 1;

	wchar_t* super = new wchar_t[size * sizeof(wchar_t)]();

	if (super == nullptr)
	{
		delete[] super;
		THROW_EXCEPTION("String concencation failed! Wide buffer is null!");
	}

	wcscpy(super, otherABuffer);
	wcscat(super, otherBBuffer);

	String newStringBuffer(super);

	delete[] super;
	return newStringBuffer;
}


bool String::operator==(const String& Other) const
{
	return wcscmp(buffer, Other.buffer) == 0;
}

bool String::operator!=(const String& Other) const
{
	return wcscmp(buffer, Other.buffer) != 0;
}

bool String::operator!=(String& Other) const
{
	return wcscmp(buffer, Other.buffer) != 0;
}

const wchar_t* String::operator*()
{
	return buffer ? buffer : L"";
}

const wchar_t* String::Chr()
{
	return buffer ? buffer : L"";
}

wchar_t* String::Data()
{
	return buffer;
}

void String::Narrow(const wchar_t* wideString, char*& narrowString)
{
	//const size_t narrowSize = wcstombs(NULL, wideString, 0);

	//char* narrowBuffer = (char*)mrmalloc((narrowSize + 1) * sizeof(char));

	//if (narrowBuffer == nullptr)
	//	THROW_EXCEPTION("Unable to convert from Wide to Narrow! Buffer is null!");

	//const size_t converted = wcstombs(narrowBuffer, wideString, narrowSize + 1);

	//if (converted == 0)
	//	THROW_EXCEPTION("No characters converted!");

	//strncpy(narrowString, narrowBuffer, narrowSize + 1);
	//mrfree(narrowBuffer);
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

String String::Delim(const String character, bool first)
{
	if (character.isEmpty())
		return "";

	wchar_t* A = buffer;
	wchar_t* B = wcstok(A, character.Chr(), &A);

	return first ? B : A;
}

bool String::isEmpty() const
{
	return buffer == nullptr || wcslen(buffer) == 0;
}

bool String::endsWith(const String string) const
{
	if (string.isEmpty())
		return false;

	size_t suffixLength = wcslen(string.Chr());
	size_t thisLength = wcslen(this->buffer);

	if (suffixLength > thisLength)
		return false;

	return wcscmp(this->Chr() + thisLength - suffixLength, string.Chr()) == 0;
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

String String::Format(const String format, ...)
{
	const wchar_t* formattingBuffer = format.Chr();

	va_list a;
	va_start(a, format);
	const uint32 sizeForVA = vswprintf(0, 0, formattingBuffer, a);
	va_end(a);

	wchar_t* newFormattedBuffer = new wchar_t[(sizeForVA + 1) * sizeof(wchar_t)]();

	va_list b;
	va_start(b, format);
	vswprintf(newFormattedBuffer, sizeForVA + 1, formattingBuffer, b);
	va_end(b);

	String stringized(newFormattedBuffer);

	delete[] newFormattedBuffer;
	return stringized;
}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		if (!other.buffer)
		{
			return *this;
		}

		if (buffer)
		{
			delete[] buffer;
			buffer = nullptr;
		};

		const size_t otherBuffSize = wcslen(other.buffer) + 1;
		buffer = new wchar_t[otherBuffSize * sizeof(wchar_t)]();
		wcsncpy(buffer, other.buffer, otherBuffSize);
	}

	return *this;
}