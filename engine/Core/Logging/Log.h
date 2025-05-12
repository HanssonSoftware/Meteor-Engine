/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <vector>

#ifdef MR_DEBUG
static constexpr const bool bIsRunningDebugMode = true;
#else
static constexpr const bool bIsRunningDebugMode = false;
#endif // MR_DEBUG

struct String;
class IFile;

struct LogEntry
{

};

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

	constexpr LogDescriptor(const char* entry, LogSeverity severity, const char* message, const char* function, const char* file, ...) noexcept
		: team(entry), severity(severity), message(message), function(function), file(file)
	{
		
	}

	const char* team;

	LogSeverity severity;

	const char* message;

	const char* function;

	const char* file;
};


struct AssertionPackage
{
	constexpr AssertionPackage(const char* error, const char* message, const char* file, unsigned int line) noexcept
		: Checked(error), Message(message), File(file), Line(line)
	{

	}

	const char* Checked;

	const char* Message;

	const char* File;

	unsigned int Line;
};

class ILogger
{
public:
	virtual void Intialize();

	virtual ~ILogger() noexcept;

	/** Prints all logs to the console window, whenever it got logged before critical.*/
	void SendPreInitLogs();

	void TransmitMessage(const LogDescriptor& Descriptor);

	void TransmitAssertion(const AssertionPackage* Info);

	virtual void SendToOutputBuffer(const String& Buffer);
protected:
	bool bIsUsingVerbose = false;

	bool bIsUsingFile = true;

	IFile* buffer = nullptr;

	std::vector<LogDescriptor> critialLogs;
};

