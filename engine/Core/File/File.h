/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Log/LogMacros.h>

LOG_ADDCATEGORY(FileSystem);

typedef enum FileOpenRules
{
	OPENRULE_READ = 1 << 0,
	OPENRULE_WRITE = 1 << 1,
	OPENRULE_DELETE = 1 << 2

} FileOpenRules;

typedef enum FileOverrideRules
{
	OVERRIDERULE_JUST_OPEN,
	OVERRIDERULE_OPEN_ONLY_IF_EXISTS,
	OVERRIDERULE_CREATE_NEW_IF_NOT_EXISTS,
	OVERRIDERULE_CREATE_NEW_DONT_MIND

} FileOverrideRules;

typedef enum FileStatus
{
	FILESTATUS_NONE,
	FILESTATUS_PROCESSING,
	FILESTATUS_NOT_FOUND,
	FILESTATUS_ALREADY_EXISTS,
	FILESTATUS_GOOD,
	FILESTATUS_ERROR,
	FILESTATUS_DIED

} FileStatus;

class FMFile
{
public:
	FMFile();

	~FMFile();

	FileStatus Open(const String Name, int openRules, FileOverrideRules overrideRules);

	void Write(const String buffer) const;

	void Read();

	String getName() const { return fileName; };

	String getExtension() const { return extension; };

	void setName(String content);

	bool isValid() const
	{
		return bWasInitSucceded;
	}
		
	/** Returns true, if the file was "created" from the hard disk. */
	bool isPersistent() const
	{
		return !bWasCreatedByProgramatically;
	}

	void Delete();

	const wchar_t* getBuffer() const noexcept
	{
		return buffer;
	}

private:
	bool bWasCreatedByProgramatically = false;

protected:
	bool bWasInitSucceded = false;

	uint32 size = 0;

	void* fileHandle;

	wchar_t* buffer;
private:

	String fileName;

	String extension;

	constexpr int evaluateOverrideRules(FileOverrideRules flags);
};

