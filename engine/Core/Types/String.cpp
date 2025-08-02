/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "String.h"
#include <Logging/LogMacros.h>
#include <cstdarg>

#pragma warning(disable : 26495)

static_assert(sizeof(char) == 1, "Advanced string handling required!");

String::String() noexcept
	: bIsUsingHeap(false)
{
	MakeEmpty();

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const char* Input)
{
	if (!Input || Input[0] == '\0')
	{
		MakeEmpty();
		return;
	}

	const size_t inputSize = strlen(Input);
	if (inputSize <= SSO_MAX_CHARS)
	{
		memcpy(stackBuffer.ptr, Input, inputSize);
		stackBuffer.ptr[inputSize] = '\0';

		stackBuffer.length = (unsigned short)inputSize;
	
		bIsUsingHeap = false;
	}
	else
	{
		heapBuffer.length = inputSize;
		heapBuffer.capacity = inputSize + 1;

		heapBuffer.ptr = new char[heapBuffer.capacity]();
		memcpy(heapBuffer.ptr, Input, inputSize);
		
		heapBuffer.ptr[inputSize] = '\0';
		
		bIsUsingHeap = true;
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const wchar_t* Input)
{
	if (!Input || Input[0] == L'\0')
	{
		MakeEmpty();
		return;
	}

	const size_t inputSize = wcslen(Input);

	if (inputSize <= SSO_MAX_CHARS)
	{
		wcstombs(stackBuffer.ptr, Input, inputSize);

		stackBuffer.ptr[inputSize] = '\0';

		stackBuffer.length = (unsigned short)inputSize;
	
		bIsUsingHeap = false;
	}
	else
	{
		heapBuffer.length = inputSize;
		heapBuffer.capacity = inputSize + 1;

		heapBuffer.ptr = new char[heapBuffer.capacity]();

		wcstombs(heapBuffer.ptr, Input, inputSize);

		heapBuffer.ptr[inputSize] = '\0';

		bIsUsingHeap = true;
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const int Input)
{
	const int inputSize = snprintf(nullptr, 0, "%d", Input);

	if (inputSize <= SSO_MAX_CHARS)
	{
		snprintf(stackBuffer.ptr, inputSize + 1, "%d", Input);

		//stackBuffer.ptr[inputSize] = '\0'; // optional

		stackBuffer.length = (unsigned short)inputSize;

		bIsUsingHeap = false;
	}
	else
	{
		heapBuffer.length = inputSize;
		heapBuffer.capacity = inputSize + 1;

		heapBuffer.ptr = new char[heapBuffer.capacity]();

		snprintf(heapBuffer.ptr, inputSize + 1, "%d", Input);

		heapBuffer.ptr[inputSize] = '\0';

		bIsUsingHeap = true;
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const uint32 Input)
{
	const int inputSize = snprintf(nullptr, 0, "%u", Input);

	if (inputSize <= SSO_MAX_CHARS)
	{
		snprintf(stackBuffer.ptr, inputSize + 1, "%u", Input);

		//stackBuffer.ptr[inputSize] = '\0'; // optional

		stackBuffer.length = (unsigned short)inputSize;

		bIsUsingHeap = false;
	}
	else
	{
		heapBuffer.length = inputSize;
		heapBuffer.capacity = inputSize + 1;

		heapBuffer.ptr = new char[heapBuffer.capacity]();

		snprintf(heapBuffer.ptr, inputSize + 1, "%u", Input);

		heapBuffer.ptr[inputSize] = '\0';

		bIsUsingHeap = true;
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const float Input)
{
	const int inputSize = snprintf(nullptr, 0, "%f", Input);

	if (inputSize <= SSO_MAX_CHARS)
	{
		snprintf(stackBuffer.ptr, inputSize + 1, "%f", Input);

		//stackBuffer.ptr[inputSize] = '\0'; // optional

		stackBuffer.length = (unsigned short)inputSize;

		bIsUsingHeap = false;
	}
	else
	{
		heapBuffer.length = inputSize;
		heapBuffer.capacity = inputSize + 1;

		heapBuffer.ptr = new char[heapBuffer.capacity]();

		snprintf(heapBuffer.ptr, inputSize + 1, "%f", Input);

		heapBuffer.ptr[inputSize] = '\0';

		bIsUsingHeap = true;
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const unsigned long Input)
{
	const int inputSize = snprintf(nullptr, 0, "%u", Input);

	if (inputSize <= SSO_MAX_CHARS)
	{
		snprintf(stackBuffer.ptr, inputSize + 1, "%u", Input);

		//stackBuffer.ptr[inputSize] = '\0'; // optional

		stackBuffer.length = (unsigned short)inputSize;

		bIsUsingHeap = false;
	}
	else
	{
		heapBuffer.length = inputSize;
		heapBuffer.capacity = inputSize + 1;

		heapBuffer.ptr = new char[heapBuffer.capacity]();

		snprintf(heapBuffer.ptr, inputSize + 1, "%u", Input);

		heapBuffer.ptr[inputSize] = '\0';

		bIsUsingHeap = true;
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const std::string Input)
{
	if (Input.empty())
	{
		MakeEmpty();
		return;
	}

	const size_t inputSize = Input.size();
	if (inputSize <= SSO_MAX_CHARS)
	{
		memcpy(stackBuffer.ptr, Input.c_str(), inputSize);
		stackBuffer.ptr[inputSize] = '\0';

		stackBuffer.length = (unsigned short)inputSize;

		bIsUsingHeap = false;
	}
	else
	{
		heapBuffer.length = inputSize;
		heapBuffer.capacity = inputSize + 1;

		heapBuffer.ptr = new char[heapBuffer.capacity]();
		memcpy(heapBuffer.ptr, Input.c_str(), inputSize);

		heapBuffer.ptr[inputSize] = '\0';

		bIsUsingHeap = true;
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const std::wstring Input)
{
	const size_t inputSize = Input.size();

	if (inputSize <= SSO_MAX_CHARS)
	{
		wcstombs(stackBuffer.ptr, Input.c_str(), inputSize);

		stackBuffer.ptr[inputSize] = '\0';

		stackBuffer.length = (unsigned short)inputSize;

		bIsUsingHeap = false;
	}
	else
	{
		heapBuffer.length = inputSize;
		heapBuffer.capacity = inputSize + 1;

		heapBuffer.ptr = new char[heapBuffer.capacity]();

		wcstombs(heapBuffer.ptr, Input.c_str(), inputSize);

		heapBuffer.ptr[inputSize] = '\0';

		bIsUsingHeap = true;
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(String&& other) noexcept
{
	ResetBuffers();
	
	bIsUsingHeap = other.bIsUsingHeap;

	if (other.bIsUsingHeap)
	{
		heapBuffer.capacity = other.heapBuffer.capacity;
		heapBuffer.length = other.heapBuffer.length;

		heapBuffer.ptr = new char[heapBuffer.capacity]();

		memmove(heapBuffer.ptr, other.heapBuffer.ptr, heapBuffer.length);
		heapBuffer.ptr[heapBuffer.length] = '\0';

		other.ResetBuffers();
	}
	else
	{
		stackBuffer.length = other.stackBuffer.length;

		memmove(stackBuffer.ptr, other.stackBuffer.ptr, stackBuffer.length);
		stackBuffer.ptr[stackBuffer.length] = '\0';

		other.ResetBuffers();
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const String& other)
{
	if (other.bIsUsingHeap)
	{
		bIsUsingHeap = true;

		heapBuffer.capacity = other.heapBuffer.capacity;
		heapBuffer.length = other.heapBuffer.length;

		heapBuffer.ptr = new char[heapBuffer.capacity]();

		memcpy(heapBuffer.ptr, other.heapBuffer.ptr, heapBuffer.length);
		heapBuffer.ptr[heapBuffer.length] = '\0';
	}
	else
	{
		bIsUsingHeap = false;

		stackBuffer.length = other.stackBuffer.length;

		memcpy(stackBuffer.ptr, other.stackBuffer.ptr, stackBuffer.length);
		stackBuffer.ptr[stackBuffer.length] = '\0';
	}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const char* string, uint32 length)
{
	MakeEmpty();

	bIsUsingHeap = length <= SSO_MAX_CHARS ? false : true;
	
	if (bIsUsingHeap)
	{
		heapBuffer.capacity = (size_t)(length + 1);
		heapBuffer.length = (size_t)length;
		heapBuffer.ptr = new char[heapBuffer.capacity]();

		memcpy(heapBuffer.ptr, string, length);
		heapBuffer.ptr[heapBuffer.length] = '\0';
	}
	else
	{
		stackBuffer.length = (unsigned short)length;

		memcpy(stackBuffer.ptr, string, length);
		stackBuffer.ptr[stackBuffer.length] = '\0';
	}
}

String::~String() noexcept
{
	if (bIsUsingHeap && heapBuffer.ptr)
	{
		delete[] heapBuffer.ptr;
		heapBuffer.ptr = nullptr;

		heapBuffer.length = 0;
		heapBuffer.capacity = 0;

		bIsUsingHeap = false;
	}

#ifdef MR_DEBUG
	bIsInited = false;
#endif // MR_DEBUG
}

String String::operator+(const String& Other)
{
	//const char* otherBuffer = Other.buffer;
	//const char* thisBuffer = buffer;

	//const size_t size = strlen(thisBuffer) + strlen(otherBuffer) + 1;

	//char* super = new char[size]();

	//if (super == nullptr)
	//{
	//	delete[] super;
	//	MR_LOG(LogTemp, Error, "String concencation failed! Wide buffer is null!");
	//	return String("");
	//}

	//strcpy(super, thisBuffer);
	//strcat(super, otherBuffer);

	//String newStringBuffer(super);

	//delete[] super;
	//return newStringBuffer;
	return String("");
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
	return strcmp(bIsUsingHeap ? heapBuffer.ptr : stackBuffer.ptr, Other.bIsUsingHeap ? Other.heapBuffer.ptr : Other.stackBuffer.ptr) == 0;
}

bool String::operator!=(const String& Other) const
{
	return strcmp(bIsUsingHeap ? heapBuffer.ptr : stackBuffer.ptr, Other.bIsUsingHeap ? Other.heapBuffer.ptr : Other.stackBuffer.ptr) != 0;
}

String String::operator+=(const String& other)
{
	if (!other.IsEmpty())
	{
		if (bIsUsingHeap)
		{
			const size_t length = Length() + (other.bIsUsingHeap ? other.heapBuffer.length : other.stackBuffer.length);

			strcat(heapBuffer.ptr, other.bIsUsingHeap ? other.heapBuffer.ptr : other.stackBuffer.ptr);

			heapBuffer.length = length;
			heapBuffer.capacity = length + 1;
		}
		else
		{
			MR_ASSERT(false, "This part is not implemented, yet!");
		}
	}

	return *this;
}

const char* String::Chr() const
{
	return bIsUsingHeap ? (heapBuffer.ptr || heapBuffer.length != 0 ? heapBuffer.ptr : "") : 
		stackBuffer.ptr || stackBuffer.length != 0 ? stackBuffer.ptr : "";
}

char* String::Allocate()
{
	return new char[bIsUsingHeap ? heapBuffer.capacity : stackBuffer.length + 1]();
}

bool String::operator!() const
{
	return bIsUsingHeap ? !heapBuffer.ptr : !stackBuffer.ptr;
}

String String::Delim(const String character, bool first)
{
	if (character.IsEmpty())
		return "";

	char* A = /*buffer*/nullptr;
	char* B = strtok(A, character.Chr());

	return first ? B : A;
}

bool String::IsEmpty() const noexcept
{
	return bIsUsingHeap ? heapBuffer.length == 0 : stackBuffer.length == 0;
}

int String::ToInt() const noexcept
{
	return strtol(bIsUsingHeap ? heapBuffer.ptr : stackBuffer.ptr, nullptr, 0);
}

float String::ToFloat() const noexcept
{
	return strtof(bIsUsingHeap ? heapBuffer.ptr : stackBuffer.ptr, nullptr);
}

uint32 String::Length() const noexcept
{
	return bIsUsingHeap ? (uint32)heapBuffer.length : (uint32)stackBuffer.length;
}

String String::Format(const String format, ...)
{
	const char* formattingBuffer = format.Chr();

	va_list a;
	va_start(a, format);

	va_list a_cpy;
	va_copy(a_cpy, a);
	const uint32 sizeForVA = vsnprintf(nullptr, 0, formattingBuffer, a_cpy);
	va_end(a_cpy);

	char* newFormattedBuffer = new char[sizeForVA + 1]();

	vsnprintf(newFormattedBuffer, sizeForVA + 1 ,formattingBuffer, a);
	va_end(a);

	String stringized(newFormattedBuffer);

	delete[] newFormattedBuffer;
	return stringized;
}

bool String::Contains(const char* buffer, const char* target)
{
	if (!buffer || !target)
		return false;

	char* dupedBuffer = _strdup(buffer);

	MR_ASSERT(strcmp(dupedBuffer, buffer) == 0, "Buffer duplication error!");

	char* deconstedParam = const_cast<char*>(target);

	char* found = strstr(dupedBuffer, deconstedParam);

	const bool result = found ? true : false;

	free(dupedBuffer);
	return result;
}

constexpr void String::MakeEmpty()
{
	stackBuffer.ptr[0] = '\0';
	stackBuffer.length = 0;
}

constexpr void String::ResetBuffers()
{
	if (bIsUsingHeap)
	{
		delete[] heapBuffer.ptr;
		heapBuffer.ptr = nullptr;

		heapBuffer.capacity = 0;
		heapBuffer.length = 0;
	}
	else
	{
		stackBuffer.ptr[0] = '\0';
		stackBuffer.length = 0;
	}
}

String String::operator=(const String& other)
{
	if (this != &other)
	{
		ResetBuffers();

		if (other.bIsUsingHeap)
		{
			bIsUsingHeap = true;

			heapBuffer.capacity = other.heapBuffer.capacity;
			heapBuffer.length = other.heapBuffer.length;

			heapBuffer.ptr = new char[heapBuffer.capacity]();
			strncpy(heapBuffer.ptr, other.heapBuffer.ptr, heapBuffer.length);
			
			heapBuffer.ptr[heapBuffer.length] = '\0';

			return *this;
		}
		else
		{
			bIsUsingHeap = false;

			stackBuffer.length = other.stackBuffer.length;

			strncpy(stackBuffer.ptr, other.stackBuffer.ptr, stackBuffer.length);

			stackBuffer.ptr[stackBuffer.length] = '\0';

			return *this;
		}
	}

	return *this;
}

bool String::IsWhitespace(const char* buffer)
{
	if (buffer[0] == '\n' || 
		buffer[0] == '\t' ||
		buffer[0] == '\f' ||
		buffer[0] == '\r' ||
		buffer[0] == '\v' )
	{
		return true;
	}

	return false;
}

bool String::IsSpace(const char* buffer)
{
	return buffer[0] == ' ' ? true : false;
}

bool String::IsAlpha(const char* buffer)
{
	return (buffer[0] >= 'a' && buffer[0] <= 'z' || buffer[0] >= 'A' && buffer[0] <= 'Z');
}
