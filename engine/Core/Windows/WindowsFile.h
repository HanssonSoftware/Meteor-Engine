/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/File.h>


class WindowsFile : public IFile
{
	friend struct WindowsFileManager;
public:
	WindowsFile() = default;

	virtual ~WindowsFile();

	virtual void* GetFileHandle();

	virtual bool ValidDirectory(const String& directory, bool bCreateIfNotExist = false) override;

	virtual void Write(const String buffer) const override;

	virtual void Read() override;

	virtual void Close() override;
private:
	void* /*HANDLE*/ fileHandle;
};

