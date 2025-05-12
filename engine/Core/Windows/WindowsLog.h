/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Logging/Log.h>

class WindowsLog : public ILogger
{
public:
	virtual void Intialize() override;

	virtual void SendToOutputBuffer(const String& Buffer) override;
};

