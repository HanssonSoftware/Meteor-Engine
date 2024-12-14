/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Platform/Microsoft/MinWin.h>
#include <Timer/Timer.h>
#include <Common/MemoryManager.h>
#include <cstdarg>
#include <fstream>
#include <vector>


enum ESeverity
{
	Info, /** Just for notifications, this will not be added to file, like found adapters, etc. */
	Log, /** Logging standard, you will use this as most. */
	Verbose, /** Ctrl+C, Ctrl+V from Info. Also this message won't be saved, only for console screen.*/
	Warn, /** User warning type, text color is different, to draw user attention to console screen.*/
	Error, /** Indicating an error, which needs some attention.*/
	Fatal /** Very dangerous situation, this will terminate the app.*/
};


struct LogPart
{
	friend class Logger;

	LogPart(String Category, ESeverity DisplayTitle, String Message, ...);

	LogPart(String Category, ESeverity DisplayTitle, String Message);
		
private:

	String time;

	String category;

	ESeverity displayTitle = Log;

	String message;

	String rawMessage;

	constexpr const wchar_t* formatSeverity(ESeverity Severity) const noexcept;
};

class Logger
{
public:
	static Logger& Get();

	void firstStartLogger();

	void shutdownLogger();

	/** Prints all logs to the console window, whenever it got logged before critical.*/
	void printCollectedLogs();

	void logMessage(LogPart Log, const wchar_t* Function, const wchar_t* File);

	void logAssert(const wchar_t* Function, const wchar_t* File, const wchar_t* Input, ...);

	inline void writeToOutput(const wchar_t* Input, bool bFiled);

	~Logger();
#ifdef _WIN32
	const wchar_t* dispatchLastError();

	const wchar_t* dispatchLastError(HRESULT result);
#endif

private:
	Logger() {};

	String formatQuickFatal(const wchar_t* message, const wchar_t* Callstack, const wchar_t* File) const;
#ifdef _WIN32
	void createConsoleW();

	constexpr const void setColorBySeverity(ESeverity Severity) const noexcept;
public:
	constexpr const wchar_t* getMessageName(unsigned int code) const noexcept;
private:
#endif // _WIN32
	std::vector<LogPart> critialLogs;

	std::wofstream stream;
};


