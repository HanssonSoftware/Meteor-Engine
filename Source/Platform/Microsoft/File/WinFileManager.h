/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#include "File/FileManager.h"

class WinFileManager : public IFileManager
{
public:

	virtual bool openFile(FileOpenInfo* details) override;

	virtual uint64 getSize(const String& name) const override;

	WinFileManager();

	virtual ~WinFileManager() override;

	virtual bool closeFile(void*& outputFile) override;
};

