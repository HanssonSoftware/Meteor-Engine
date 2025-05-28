/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "File.h"
#include <Logging/LogMacros.h>

IFile::~IFile()
{
	if (buffer != nullptr)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}

void IFile::Delete()
{
	if (!bWasInitSucceded)
		return;
}

void IFile::Write(const String buffer) const
{
	if (!bWasInitSucceded)
		return;
}

void IFile::Read()
{
	if (!bWasInitSucceded)
		return;
}
