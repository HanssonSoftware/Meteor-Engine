/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Exception.h"
#include <cstdarg>
#include <Application/Application.h>
#include <Common/MemoryManager.h>

LOG_ADDCATEGORY(Exception);

Exception::Exception(String text, int line, const wchar_t* File ,const wchar_t* function, ...)
	: description(text)
{
#ifdef MR_DEBUG
	va_list args;
	va_start(args, function);
	const uint32 requiredVariadicAmount = vswprintf(NULL, 0, text.Chr(), args) + 1;

	wchar_t* variadicBuffer = (wchar_t*)mrmalloc(requiredVariadicAmount * sizeof(wchar_t));
	vswprintf(variadicBuffer, requiredVariadicAmount, text.Chr(), args);
	va_end(args);

	MR_LOG(LogException, Error, TEXT("%ls"), variadicBuffer);
	mrfree(variadicBuffer);
#else
	va_list args;
	va_start(args, function);
	const uint32 requiredVariadicAmount = vswprintf(NULL, 0, text.Chr(), args) + 1; // L"One or more Unhandled Exception(s) are occourred.\n\n%s\nAt:%s, Line: %d\nPress OK to close the application!"

	wchar_t* variadicBuffer = (wchar_t*)mrmalloc(requiredVariadicAmount * sizeof(wchar_t));
	vswprintf(variadicBuffer, requiredVariadicAmount, text.Chr(), args);
	va_end(args);

	const uint32 requiredFullAmount = swprintf(NULL, 0, L"One or more Unhandled Exception(s) are occourred.\n\n%s\n\n\nAt: %s, Line: %d\nPress OK to close the application!", variadicBuffer, File, line) + 1;

	wchar_t* fullBuffer = (wchar_t*)mrmalloc((requiredFullAmount + requiredVariadicAmount) * sizeof(wchar_t));
	swprintf(fullBuffer, (requiredFullAmount + requiredVariadicAmount), L"One or more Unhandled Exception(s) are occourred.\n\n%s\n\n\nAt: %s, Line: %d\nPress OK to close the application!", variadicBuffer, File, line);

	String super(fullBuffer);

	mrfree(fullBuffer);
	mrfree(variadicBuffer);
	MessageBox(NULL, super.Chr(), Application::Get().getApplicationName().Chr(), MB_ICONERROR | MB_OK);
#endif // MR_DEBUG
}

Exception::Exception(String text) 
	: description(text)
{
	MR_LOG(LogException, Error, TEXT("Exception: %s"), *description);
}

bool Exception::operator==(const Exception& other) const
{
	return description == other.description;
}
