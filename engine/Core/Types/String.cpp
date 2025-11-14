/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "String.h"
#include <Logging/Log.h>
#include <stdio.h>

#include <Layers/SystemLayer.h>

#include <Platform/Platform.h>
#include <Platform/PlatformLayout.h>
#include <Windows/Windows.h>

#include <MemoryManager.h>
#include "Pointers.h"

#pragma warning(disable : 26495)

LOG_ADDCATEGORY(StringSet);


String::~String() noexcept
{
	if (bIsUsingHeap && heapBuffer.ptr)
		MemoryManager::Get().Deallocate<wchar_t>(heapBuffer.ptr);

	NullOut();

#ifdef MR_DEBUG
	bIsInited = false;
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
			MR_LOG(LogStringSet, Error, "MultiByteToWideChar returned: %s", *Platform::GetError());
			return;
		}
	}
	else
	{
		MR_LOG(LogStringSet, Error, "MultiByteToWideChar returned: %s", *Platform::GetError());
		return;
	}
#endif // MR_PLATFORM_WINDOWS

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const wchar_t* Input)
{
	NullOut();

	if (!Input || *Input == '\0')
		return;

	const uint32_t size = wcslen(Input);
	wchar_t* target = DetermineLocation(size);
	
	wcsncpy(target, Input, size);

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(int Input)
{
	swprintf(stackBuffer.ptr, 14, L"%d", Input);

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(uint32_t Input)
{
	swprintf(stackBuffer.ptr, 14, L"%ud", Input);

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(float Input)
{
	swprintf(stackBuffer.ptr, 14, L"%ud", Input);

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(String&& other) noexcept
{
	ResetBuffers();
	
	bIsUsingHeap = other.bIsUsingHeap;

	//if (other.bIsUsingHeap)
	//{
	//	heapBuffer.capacity = other.heapBuffer.capacity;
	//	heapBuffer.length = other.heapBuffer.length;

	//	heapBuffer.ptr = MemoryManager::Get().Allocate<char>(heapBuffer.capacity);

	//	memmove(heapBuffer.ptr, other.heapBuffer.ptr, heapBuffer.length);
	//	heapBuffer.ptr[heapBuffer.length] = '\0';

	//	other.ResetBuffers();
	//}
	//else
	//{
	//	stackBuffer.length = other.stackBuffer.length;

	//	memmove(stackBuffer.ptr, other.stackBuffer.ptr, stackBuffer.length);
	//	stackBuffer.ptr[stackBuffer.length] = '\0';

	//	other.ResetBuffers();
	//}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const String& other)
{
	//if (other.bIsUsingHeap)
	//{
	//	bIsUsingHeap = true;

	//	heapBuffer.capacity = other.heapBuffer.capacity;
	//	heapBuffer.length = other.heapBuffer.length;

	//	heapBuffer.ptr = MemoryManager::Get().Allocate<char>(heapBuffer.capacity);

	//	memcpy(heapBuffer.ptr, other.heapBuffer.ptr, heapBuffer.length);
	//	heapBuffer.ptr[heapBuffer.length] = '\0';
	//}
	//else
	//{
	//	bIsUsingHeap = false;

	//	stackBuffer.length = other.stackBuffer.length;

	//	memcpy(stackBuffer.ptr, other.stackBuffer.ptr, stackBuffer.length);
	//	stackBuffer.ptr[stackBuffer.length] = '\0';
	//}

#ifdef MR_DEBUG
	bIsInited = true;
#endif // MR_DEBUG
}

String::String(const wchar_t* string, uint32_t length)
{
	MakeEmpty();

	bIsUsingHeap = length <= SSO_MAX_CHARS ? false : true;
	
	//if (bIsUsingHeap)
	//{
	//	heapBuffer.capacity = (uint32_t)(length + 1);
	//	heapBuffer.length = (uint32_t)length;
	//	heapBuffer.ptr = MemoryManager::Get().Allocate<char>(heapBuffer.capacity);

	//	memcpy(heapBuffer.ptr, string, length);
	//	heapBuffer.ptr[heapBuffer.length] = '\0';
	//}
	//else
	//{
	//	stackBuffer.length = (unsigned short)length;

	//	memcpy(stackBuffer.ptr, string, length);
	//	stackBuffer.ptr[stackBuffer.length] = '\0';
	//}
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

wchar_t* String::Allocate()
{
	wchar_t* buffer = MemoryManager::Get().Allocate<wchar_t>(bIsUsingHeap ? heapBuffer.capacity : stackBuffer.length + 1);
	wmemcpy(buffer, Chr(), bIsUsingHeap ? heapBuffer.length : stackBuffer.length);
	
	return buffer;
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

void String::MakeEmpty()
{
	memset(stackBuffer.ptr, 0, SSO_MAX_CHARS);
	stackBuffer.length = 0;
}

void String::ResetBuffers()
{
	if (bIsUsingHeap)
	{
		MemoryManager::Get().Deallocate<wchar_t>(heapBuffer.ptr);
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

wchar_t* String::DetermineLocation(uint32_t size)
{
	bIsUsingHeap = size > SSO_MAX_CHARS;
	if (bIsUsingHeap)
	{
		heapBuffer.capacity = size * 2;
		heapBuffer.ptr = MemoryManager::Get().Allocate<wchar_t>(heapBuffer.capacity * sizeof(wchar_t));
		heapBuffer.length = size;

		memset(heapBuffer.ptr, '\0', heapBuffer.capacity);
		return heapBuffer.ptr;
	}

	stackBuffer.length = size;
	return stackBuffer.ptr;
}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		ResetBuffers();

		//if (other.bIsUsingHeap)
		//{
		//	bIsUsingHeap = true;

		//	heapBuffer.capacity = other.heapBuffer.capacity;
		//	heapBuffer.length = other.heapBuffer.length;

		//	heapBuffer.ptr = MemoryManager::Get().Allocate<char>(heapBuffer.capacity);
		//	strncpy(heapBuffer.ptr, other.heapBuffer.ptr, heapBuffer.length);
		//	
		//	heapBuffer.ptr[heapBuffer.length] = '\0';

		//	return *this;
		//}
		//else
		//{
		//	bIsUsingHeap = false;

		//	stackBuffer.length = other.stackBuffer.length;

		//	strncpy(stackBuffer.ptr, other.stackBuffer.ptr, stackBuffer.length);

		//	stackBuffer.ptr[stackBuffer.length] = '\0';

		//	return *this;
		//}
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