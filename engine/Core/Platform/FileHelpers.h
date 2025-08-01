/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once

enum FileAccessMode
{
	OPENMODE_NONE = 0 << 0,
	OPENMODE_READ = 1 << 0,
	OPENMODE_WRITE = 1 << 1,
};

enum FileShareMode
{
	SHAREMODE_NONE = 0 << 0,
	SHAREMODE_READ = 1 << 0,
	SHAREMODE_WRITE = 1 << 1,
	SHAREMODE_DELETE = 1 << 2
};

enum FileOverrideRules
{
	OVERRIDERULE_OPEN_ALWAYS,
	OVERRIDERULE_OPEN_ONLY_IF_EXISTS,
	OVERRIDERULE_CREATE_NEW_IF_NOT_EXISTS,
	OVERRIDERULE_CREATE_NEW_DONT_MIND
};

enum FileStatus
{
	FILESTATUS_NONE,
	FILESTATUS_PROCESSING,
	FILESTATUS_NOT_FOUND,
	FILESTATUS_ALREADY_EXISTS,
	FILESTATUS_GOOD,
	FILESTATUS_ERROR,
	FILESTATUS_DIED
};
