/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Platform/File.h>


class WindowsFile : public IFile
{
public:
	WindowsFile() = default;

	virtual ~WindowsFile();

	virtual void* GetFileHandle() const;

	virtual FileStatus Open(const String Name, int openRules, FileOverrideRules overrideRules) override;

	virtual bool ValidDirectory(const String& directory, bool bCreateIfNotExist = false) override;

	virtual void Write(const String buffer) const override;

	virtual void Read() override;

	virtual void Close() override;
private:
	void* /*HANDLE*/ fileHandle;

	virtual int EvaluateOverrideRules(FileOverrideRules flags) const override;
};

