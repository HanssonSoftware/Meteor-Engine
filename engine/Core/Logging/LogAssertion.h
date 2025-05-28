/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>

struct LogAssertion
{
	friend struct ILogger;

	LogAssertion() = delete;

	virtual ~LogAssertion() noexcept = default;

	LogAssertion(const char* inputFile, const int inputLine, const char* statement, const char* inputMessage, ...);

	const char* assertLocationInFile;

	const char* assertStatement;

	String assertMessage;

	unsigned int assertLineInFile = 0;

	unsigned int bIgnoreFor = 0;

	bool bIgnoreThis = false;
};

