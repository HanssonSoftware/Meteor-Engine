/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <cstdio>
#include <stdarg.h>
#include <Types/String.h>

enum LogSeverity : short
{
	Log, /** Logging standard, you will use this as most. */
	Verbose, /** Ctrl+C, Ctrl+V from Info. Also this message won't be saved, only for console screen.*/
	Warn, /** User warning type, text color is different, to draw user attention to console screen.*/
	Error, /** Indicating an error, which needs some attention.*/
	Fatal /** Very dangerous situation, this will terminate the app.*/
};


struct LogDescriptor
{
	LogDescriptor() = default;

	LogDescriptor(const char* entry, LogSeverity severity,const char* Message, const char* function, const char* file, ...) noexcept
		: team(entry), severity(severity), function(function), file(file)
	{
		va_list d = nullptr;
		va_start(d, file);
		const int32_t reqAmount = vsnprintf(nullptr, 0, Message, d);

		char* big = new char[reqAmount + 1]();
		vsnprintf(big, reqAmount + 1, Message, d);
		va_end(d);

		message = big;
		delete[] big;
	}

	const char* team;

	LogSeverity severity;

	String message;

	const char* function;

	const char* file;
};


