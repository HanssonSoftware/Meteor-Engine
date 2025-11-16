/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "String.h"
#include <Logging/Log.h>
//#include <stdio.h>

#include <Layers/SystemLayer.h>

#include <Platform/Platform.h>
#include <Platform/PlatformLayout.h>
#include <Windows/Windows.h>

#include <MemoryManager.h>
//#include "Pointers.h"

#pragma warning(disable : 26495)

LOG_ADDCATEGORY(StringSet);


String::~String() noexcept
{
	if (bIsUsingHeap && heapBuffer.ptr)
		MemoryManager::Get().Deallocate<wchar_t>(heapBuffer.ptr);

	NullOut();

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(const char* Input)
{
	NullOut();

	if (!Input || *Input == '\0')
		return;

#ifdef MR_PLATFORM_WINDOWS
	const uint32_t skinnyLength = (uint32_t)MultiByteToWideChar(CP_UTF8, 0, Input, -1, nullptr, 0);
	if (skinnyLength > 0)
	{
		wchar_t* redirectedData = DetermineLocation(skinnyLength);
		if (!MultiByteToWideChar(CP_UTF8, 0, Input, skinnyLength * sizeof(char), redirectedData, skinnyLength))
		{
			MR_LOG(LogStringSet, Error, "MultiByteToWideChar returned: %ls", *Platform::GetError());
			return;
		}
	}
	else
	{
		MR_LOG(LogStringSet, Error, "MultiByteToWideChar returned: %ls", *Platform::GetError());
		return;
	}
#endif // MR_PLATFORM_WINDOWS

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(const wchar_t* Input)
{
	NullOut();

	if (!Input || *Input == L'\0')
		return;

	const uint32_t size = (uint32_t)wcslen(Input);
	wchar_t* target = DetermineLocation(size);
	
	wcsncpy(target, Input, size);

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(int Input)
{
	swprintf(stackBuffer.ptr, SSO_MAX_CHARS, L"%d", Input);

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(uint32_t Input)
{
	swprintf(stackBuffer.ptr, SSO_MAX_CHARS, L"%ud", Input);

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(float Input)
{
	swprintf(stackBuffer.ptr, SSO_MAX_CHARS, L"%f", Input);

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(String&& other) noexcept
{
	NullOut();

	bIsUsingHeap = other.bIsUsingHeap;

	if (bIsUsingHeap)
	{
		heapBuffer.capacity = other.heapBuffer.capacity;
		heapBuffer.length = other.heapBuffer.length;
	}
	else
	{
		stackBuffer.length = other.stackBuffer.length;
	}

	wchar_t* determined = DetermineLocation(Length());
	wmemmove(determined, other, Length());

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(const String& other)
{
	NullOut();

	bIsUsingHeap = other.bIsUsingHeap;
	if (bIsUsingHeap)
	{
		heapBuffer.capacity = other.heapBuffer.capacity;
		heapBuffer.length = other.heapBuffer.length;
		heapBuffer.ptr = MemoryManager::Get().Allocate<wchar_t>(heapBuffer.capacity * sizeof(wchar_t));

		memset(heapBuffer.ptr, 0, heapBuffer.capacity);
		wcsncpy(heapBuffer.ptr, other.heapBuffer.ptr, heapBuffer.length);
	}
	else
	{
		stackBuffer.length = other.stackBuffer.length;

		memset(stackBuffer.ptr, 0, stackBuffer.length);
		wcsncpy(stackBuffer.ptr, other.stackBuffer.ptr, stackBuffer.length);
	}

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(const char* Input, uint32_t length)
{
	NullOut();

	if (!Input || *Input == '\0' || length <= 0)
		return;

#ifdef MR_PLATFORM_WINDOWS
	const uint32_t skinnyLength = (uint32_t)MultiByteToWideChar(CP_UTF8, 0, Input, length, nullptr, 0);
	if (skinnyLength > 0)
	{
		wchar_t* redirectedData = DetermineLocation(skinnyLength);
		if (!MultiByteToWideChar(CP_UTF8, 0, Input, skinnyLength * sizeof(char), redirectedData, skinnyLength))
		{
			MR_LOG(LogStringSet, Error, "MultiByteToWideChar returned: %ls", *Platform::GetError());
			return;
		}
	}
	else
	{
		MR_LOG(LogStringSet, Error, "MultiByteToWideChar returned: %ls", *Platform::GetError());
		return;
	}
#endif // MR_PLATFORM_WINDOWS

#ifdef MR_DEBUG
	bIsInited = true;
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}

String::String(const wchar_t* string, uint32_t length)
{
	NullOut();
	
	wchar_t* direct = DetermineLocation(length);
	wcsncpy(direct, string, length);

#ifdef MR_DEBUG
	bIsInited = true;	
	functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
}


String String::operator+(const String& Other)
{
	//const char* thisData = Data();
	//const char* otherData = Other.Chr();

	//const uint32_t thisSize = strlen(thisData);
	//const uint32_t otherSize = strlen(otherData);

	//this->bIsUsingHeap = thisSize + otherSize > SSO_MAX_CHARS ? true : false;

	//if (this->bIsUsingHeap)
	//{
	//	ScopedPtr<char> newBuffer = MemoryManager::Get().Allocate<char>(thisSize + otherSize + 1u);
	//	memcpy(newBuffer.Get(), thisData, thisSize);
	//	memcpy(newBuffer.Get() + thisSize, otherData, otherSize);


	//	thisData = newBuffer.Get();
	//}
	//else
	//{

	//}

	return *this;
}

String operator+(const String& OtherA, const String& OtherB)
{
	//// Querry the buffers.
	//const char* otherABuffer = OtherA.Chr();
	//const char* otherBBuffer = OtherB.Chr();

	//const uint32_t size = (uint32_t)(strlen(otherABuffer) + strlen(otherBBuffer) + 1);

	//char* super = MemoryManager::Get().Allocate<char>(size);

	//if (super == nullptr)
	//{
	//	delete[] super;
	//	MR_LOG(LogTemp, Error, "String concencation failed!");
	//	return String("");
	//}

	//strcpy(super, otherABuffer);
	//strcat(super, otherBBuffer);

	//String newStringBuffer(super);

	return "newStringBuffer";
}

String String::Delim(const String character, bool first)
{
	if (character.IsEmpty())
		return "";

	//char* A = /*buffer*/nullptr;
	//char* B = strtok(A, character.Chr());
	return "";
	//return first ? B : A;
}

String String::Format(const String& format, ...)
{
	va_list a;
	va_start(a, format.Chr());

	const wchar_t* formattingBuffer = format.Chr();

	va_list a_cpy;
	va_copy(a_cpy, a);
	const int sizeForVA = vswprintf(nullptr, 0, formattingBuffer, a_cpy);
	va_end(a_cpy);

	wchar_t* newFormattedBuffer = MemoryManager::Get().Allocate<wchar_t>(sizeForVA + 1);

	const int result = vswprintf(newFormattedBuffer, sizeForVA + 1 ,formattingBuffer, a);
	va_end(a);

	String stringized(newFormattedBuffer);

	MemoryManager::Get().Deallocate(newFormattedBuffer);
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

wchar_t* String::DetermineLocation(uint32_t size)
{
	bIsUsingHeap = size > SSO_MAX_CHARS;
	if (bIsUsingHeap)
	{
		heapBuffer.capacity = size * 2;
		heapBuffer.ptr = MemoryManager::Get().Allocate<wchar_t>(heapBuffer.capacity * sizeof(wchar_t));
		heapBuffer.length = size;

		wmemset(heapBuffer.ptr, 0, heapBuffer.capacity);
		return heapBuffer.ptr;
	}

	stackBuffer.length = size;
	return stackBuffer.ptr;
}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		bIsUsingHeap = other.bIsUsingHeap;
		if (bIsUsingHeap)
		{
			heapBuffer.capacity = other.heapBuffer.capacity;
			heapBuffer.length = other.heapBuffer.length;
			heapBuffer.ptr = MemoryManager::Get().Allocate<wchar_t>(heapBuffer.capacity * sizeof(wchar_t));

			memset(heapBuffer.ptr, 0, heapBuffer.capacity);
			wcsncpy(heapBuffer.ptr, other.heapBuffer.ptr, heapBuffer.length);
		}
		else
		{
			stackBuffer.length = other.stackBuffer.length;
			
			memset(stackBuffer.ptr, 0, stackBuffer.length);
			wcsncpy(stackBuffer.ptr, other.stackBuffer.ptr, stackBuffer.length);
		}

#ifdef MR_DEBUG
		bIsInited = true;
		functionWhereWasInited = __FUNCSIG__;
#endif // MR_DEBUG
	}

	return *this;
}

String& String::operator+=(const String& other)
{
	if (!other.IsEmpty())
	{
		/*const char* source = other.Chr();
		char* destination = Data();

		const uint32_t size = other.Length() + Length() + 1;

		if (bIsUsingHeap && heapBuffer.capacity < size)
		{
			heapBuffer.capacity = size * 2;
			char* newBuffer = MemoryManager::Get().Allocate<char>(heapBuffer.capacity);
			memset(newBuffer, 0, heapBuffer.capacity);

			strncpy(newBuffer, destination, heapBuffer.length);
			strncpy(newBuffer + Length(), source, other.Length());
			heapBuffer.length = size - 1;

			delete[] heapBuffer.ptr;
			heapBuffer.ptr = newBuffer;
		}
		else if (bIsUsingHeap && heapBuffer.capacity >= size)
		{
			strncpy(heapBuffer.ptr + Length(), source, other.Length());
			
			int j = 434;
		}
		else if (!bIsUsingHeap && SSO_MAX_CHARS >= size)
		{
			strncpy(stackBuffer.ptr + Length(), source, other.Length());
			stackBuffer.length = size - 1;
		}
		else if (SSO_MAX_CHARS < size)
		{
			const uint32_t stackLength = Length();

			heapBuffer.capacity = size * 2;
			char* buffer = MemoryManager::Get().Allocate<char>(size);

			memset(buffer, 0, size);

			strncpy(buffer, destination, stackLength);
			strncpy(buffer + stackLength, source, other.Length());
		
			heapBuffer.length = size - 1;

			heapBuffer.ptr = buffer;
			bIsUsingHeap = true;
		}*/
	}
	
	return *this;
}