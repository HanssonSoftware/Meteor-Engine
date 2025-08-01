/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#if defined(_WIN32) || defined(_WIN64)

#ifdef _WINDOWS_
#error Windows.h should not be included!
#endif // _WINDOWS_


#define	WIN32_LEAN_AND_MEAN
#define	NOMINMAX
#define	NOHELP
#define	NOSERVICE
#define	NOPROFILER
#define	NOMCX
#define	NODRAWTEXT
#include <Windows.h>

#endif // defined(_WIN32) || defined(_WIN64)