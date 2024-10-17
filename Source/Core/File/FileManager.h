/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include "File.h"

LOG_ADDCATEGORY(GenericFileManager);

typedef enum
{
	Open = 0,
	OpenIfExists = 1
} FileOpenRules;

typedef struct 
{
	String name;

	FileOpenRules openRules;

	void* outputFile;
} FileOpenInfo;

class IFileManager
{
public:
	/** Opens a file by their name, extension must be appended!*/
	virtual bool openFile(FileOpenInfo* details) = 0;

	/** Gets the file size and returns.*/
	virtual uint64 getSize(const String& name) const = 0;

	/** Close the file, before closing your work is saved.*/
	virtual bool closeFile(void*& outputFile) = 0;

	static IFileManager& Get();

	IFileManager() noexcept;

	virtual ~IFileManager() noexcept;
};