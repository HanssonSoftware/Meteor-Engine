﻿/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/File.h>


class WindowsFile : public IFile
{
	friend struct WindowsFileManager;
public:
	WindowsFile() = default;

	virtual ~WindowsFile() noexcept;

	virtual void* GetFileHandle();

	virtual void Write(const String& buffer) const override;

	virtual void Read() override;

	virtual void Close() override;

	virtual void Delete() override;
private:
	void* /*HANDLE*/ fileHandle;
};

