/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

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

class String;

struct LogPart
{
	friend class Logger;

	inline LogPart(String Category, ESeverity DisplayTitle, String Message, String location, String function,...);

	//LogPart(String Category, ESeverity DisplayTitle, String Message);
		
private:

	String time;

	String category;

	String message;

	String rawMessage;

	String func;

	String file;

	ESeverity displayTitle = Log;

	constexpr const wchar_t* formatSeverity(ESeverity Severity) const noexcept;
};

typedef enum LoggingLevel
{
	LOGGING_LEVEL_DISABLED = 0,
	LOGGING_LEVEL_BASIC_WO_SERIALIZATION = 1,
	LOGGING_LEVEL_BASIC = 2,
	LOGGING_LEVEL_MAXIMUM = 3
} LoggingLevel;

class Logger
{
public:
	~Logger();
	
	static Logger& Get();

	void firstStartLogger();

	void shutdownLogger();

	/** Prints all logs to the console window, whenever it got logged before critical.*/
	void printCollectedLogs();

	inline void logMessage(LogPart Log, const wchar_t* Function, const wchar_t* File);

	void logAssert(const wchar_t* Function, const wchar_t* File, const wchar_t* Input, ...);

	inline void writeToOutput(const wchar_t* Input, bool bFiled);

	constexpr inline LoggingLevel getLoggingLevel() const noexcept { return loggingState; }
#ifdef _WIN32
	const wchar_t* dispatchLastError();

	const wchar_t* dispatchLastError(HRESULT result);
#endif

private:
	LoggingLevel loggingState = LOGGING_LEVEL_BASIC;

	Logger() {};

	String formatQuickFatal(const wchar_t* message, const wchar_t* Callstack, const wchar_t* File) const;
#ifdef _WIN32
	void createConsoleW();

	inline constexpr const void setColorBySeverity(ESeverity Severity) const noexcept;
public:
	inline constexpr const wchar_t* getMessageName(unsigned int code) const noexcept;
private:
#endif // _WIN32
	std::vector<LogPart> critialLogs;

	std::wofstream stream;
};


constexpr const wchar_t* Logger::getMessageName(unsigned int code) const noexcept
{
	switch (code)
	{
	case WM_CREATE:
		return L"WM_CREATE";
	case WM_DESTROY:
		return L"WM_DESTROY";
	case WM_CLOSE:
		return L"WM_CLOSE";
	case WM_SIZING:
		return L"WM_SIZING";
	case WM_WINDOWPOSCHANGING:
		return L"WM_WINDOWPOSCHANGING";
	case WM_WINDOWPOSCHANGED:
		return L"WM_WINDOWPOSCHANGED";
	default:
		return L"";
	}

	return L"";
}