/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "String.h"
#include <Logging/LogMacros.h>
#include <cstdarg>

String::String() noexcept
{
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const char* Input)
{
	if (Input == nullptr)
		return;

	const size_t inputSize = strlen(Input);
	//if (inputSize <= MAX_STRING_SIZE)
	//{
	//	buffer.finite;
	//}

	buffer = new char[inputSize + 1]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (buffer)
	{
		strncpy(buffer, Input, inputSize);
	}
	else
	{
		MR_LOG(LogTemp, Error, "Invalid Input!");
	}
}


String::String(wchar_t* Input)
{
	if (Input == nullptr)
		return;

	const size_t requiredSize = wcslen(Input) + 1;
	if (requiredSize == 0)
		return;

	buffer = new char[requiredSize]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG

	if (wcstombs(buffer, Input, requiredSize - 1) == requiredSize - 1)
	{
		//buffer[requiredSize] = '\0';
	}
	else
	{
		delete[] buffer;
	}
}


String::String(const wchar_t* Input)
{
	if (Input == nullptr)
		return;

	const size_t requiredSize = wcslen(Input) + 1;
	if (requiredSize == 0)
		return;

	buffer = new char[requiredSize]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG

	if (wcstombs(buffer, Input, requiredSize - 1) == requiredSize - 1)
	{
		//buffer[requiredSize] = '\0';
	}
	else
	{
		delete[] buffer;
	}
}

String::String(const int Input)
{
	const std::string fake = std::to_string(Input);

	const char* convertedString = fake.c_str();
	buffer = new char[strlen(convertedString) + 1]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (strcmp(strcpy(buffer, convertedString), convertedString) == 0)
	{
		buffer[strlen(convertedString) + 1] = '\0';
	}
	else
	{
		delete[] buffer;
	}
}

String::String(const uint32 Input)
{
	const std::string fake = std::to_string(Input);

	const char* convertedString = fake.c_str();
	const uint32 size = (uint32)fake.size();

	buffer = new char[size + 1]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (strcmp(strncpy(buffer, convertedString, size), convertedString) == 0)
	{
		//buffer[strlen(convertedString) + 1] = '\0';
	}
	else
	{
		delete[] buffer;
	}
}

String::String(const float Input)
{
	const std::string fake = std::to_string(Input);

	const char* convertedString = fake.c_str();
	buffer = new char[strlen(convertedString) + 1]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (strcmp(strcpy(buffer, convertedString), convertedString) == 0)
	{
		buffer[strlen(convertedString) + 1] = '\0';
	}
	else
	{
		delete[] buffer;
	}
}

String::String(const unsigned long Input)
{
	const std::string fake = std::to_string(Input);

	const char* convertedString = fake.c_str();
	buffer = new char[strlen(convertedString) + 1]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (strcmp(strcpy(buffer, convertedString), convertedString) == 0)
	{
		buffer[strlen(convertedString) + 1] = '\0';
	}
	else
	{
		delete[] buffer;
	}
}

String::String(const std::string Input)
{
	const char* convertedString = Input.c_str();
	buffer = new char[strlen(convertedString) + 1]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
	if (strcmp(strncpy(buffer, convertedString, Input.size()), convertedString) == 0)
	{
		//buffer[strlen(convertedString) + 1] = '\0';
	}
	else
	{
		delete[] buffer;
	}
}

String::String(const std::wstring Input)
{
	if (buffer == nullptr)
		return;

	const wchar_t* convertedString = Input.c_str();
	const size_t requiredSize = wcslen(convertedString) + 1;
	if (requiredSize == 0)
		return;

	buffer = new char[requiredSize]();
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG

	if (wcstombs(buffer, convertedString, requiredSize) == requiredSize - 1)
	{
		//buffer[requiredSize] = '\0';
	}
	else
	{
		delete[] buffer;
	}
}

String::String(String&& other) noexcept
{
	
}

String::String(const String& other)
{
	if (!other.buffer)
		return;

	const char* convertedString = other.buffer;
	const int size = (int)strlen(convertedString) + 1;

	buffer = new char[size]();

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG

	strncpy(buffer, convertedString, size - 1);
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
	const char* otherBuffer = Other.buffer;
	const char* thisBuffer = buffer;

	const size_t size = strlen(thisBuffer) + strlen(otherBuffer) + 1;

	char* super = new char[size]();

	if (super == nullptr)
	{
		delete[] super;
		MR_LOG(LogTemp, Error, "String concencation failed! Wide buffer is null!");
		return String("");
	}

	strcpy(super, thisBuffer);
	strcat(super, otherBuffer);

	String newStringBuffer(super);

	delete[] super;
	return newStringBuffer;
}


String operator+(const String& OtherA, const String& OtherB)
{
	// Querry the buffers.
	const char* otherABuffer = OtherA.Chr();
	const char* otherBBuffer = OtherB.Chr();

	const size_t size = strlen(otherABuffer) + strlen(otherBBuffer) + 1;

	char* super = new char[size]();

	if (super == nullptr)
	{
		delete[] super;
		MR_LOG(LogTemp, Error, "String concencation failed!");
		return String("");
	}

	strcpy(super, otherABuffer);
	strcat(super, otherBBuffer);

	String newStringBuffer(super);

	delete[] super;
	return newStringBuffer;
}


bool String::operator==(const String& Other) const
{
	return strcmp(buffer, Other.buffer) == 0;
}

bool String::operator!=(const String& Other) const
{
	return strcmp(buffer, Other.buffer) != 0;
}

bool String::operator!=(String& Other) const
{
	return strcmp(buffer, Other.buffer) != 0;
}

const char* String::operator*()
{
	return buffer ? buffer : "";
}

const char* String::Chr()
{
	return buffer ? buffer : "";
}

char* String::Data()
{
	return buffer;
}

const char* String::Chr() const
{
	return buffer ? buffer : "";
}

String String::Delim(const String character, bool first)
{
	if (character.IsEmpty())
		return "";

	char* A = buffer;
	char* B = strtok(A, character.Chr());

	return first ? B : A;
}

bool String::IsEmpty() const
{
	return buffer == nullptr || strlen(buffer) == 0;
}

bool String::EndsWith(const String string) const
{
	if (string.IsEmpty())
		return false;

	size_t suffixLength = strlen(string.Chr());
	size_t thisLength = strlen(this->buffer);

	if (suffixLength > thisLength)
		return false;

	return strcmp(this->Chr() + thisLength - suffixLength, string.Chr()) == 0;
}

int String::ToInt() const
{
	char* end;
	return strtol(buffer, &end, 10);
}

float String::ToFloat() const
{
	char* end;
	return strtof(buffer, &end);
}

uint32 String::Length() const
{
	return buffer != nullptr ? (uint32)strlen(buffer) : /*(uint32)*/ 0;
}

String String::Format(const String format, ...)
{
	const char* formattingBuffer = format.Chr();

	va_list a;
	va_start(a, format);
	const uint32 sizeForVA = vsnprintf(0, 0, formattingBuffer, a);
	va_end(a);

	char* newFormattedBuffer = new char[sizeForVA + 1]();

	va_list b;
	va_start(b, format);
	vsnprintf(newFormattedBuffer, sizeForVA + 1 ,formattingBuffer, b);
	va_end(b);

	String stringized(newFormattedBuffer);

	delete[] newFormattedBuffer;
	return stringized;
}

inline String String::ReadLine(const String line, uint32 location)
{
	return ReadLine(line.Chr(), location);
}

String String::ReadLine(const wchar_t* Line, uint32 location)
{
	uint32 current = location;

#ifdef _WIN64
	while (Line[current] != L'\r')
#else
	while (Line[current] != L'\n')
#endif // _WIN64
	{
		current++;
	}

	wchar_t* temp = new wchar_t[(current - location) + 1];
	wcsncpy(temp, Line + location, current - location);
	temp[current - location] = L'\0';

#ifdef _WIN64
	current++;
#endif // _WIN64

	String buffer(temp);

	location = current;

	delete[] temp;
	return buffer;
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

		const size_t otherBuffSize = strlen(other.buffer) + 1;
		buffer = new char[otherBuffSize]();
		strncpy(buffer, other.buffer, otherBuffSize - 1);
	}

	return *this;
}