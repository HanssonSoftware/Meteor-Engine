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

class File
{
public:
	File(const File&) = delete;

	File() = delete;

	~File();

	File(const String Name, int openRules, FileOverrideRules overrideRules);

	void Write(const String buffer) const;

	void Read();

	String getName() const { return fileName; };

	void setName(String content);
protected:
	uint32 size = 0;

#ifdef _WIN32
	HANDLE fileHandle;
#else
	void* fileHandle;
#endif // DEBUG

	wchar_t* buffer;

	bool bWasInitSucceded = false;
private:
	String fileName;

	String extension;

	inline constexpr const int evaluateOverrideRules(FileOverrideRules flags);
};