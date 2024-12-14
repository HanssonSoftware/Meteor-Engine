/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Log/Log.h>

#include <type_traits>
#include <assert.h>

typedef struct SLogEntry{} SLogEntry;



/** Creates a logging category, do NOT insert Log before, just the word (automatically appends a Log)! */
#define LOG_ADDCATEGORY(CategoryName) \
    /* Generated log name! */ \
    struct Log##CategoryName : public SLogEntry \
	{ \
		static const wchar_t* GetCategoryName() { return L#CategoryName; } \
	};

/** Creates a static logging category which is not visible, only where you declared, do NOT insert Log before just the word (automatically appends a Log)! */
#define STATIC_LOG_ADDCATEGORY(CategoryName) \
    /* Generated log name! */ \
    static struct Log##CategoryName : public SLogEntry \
	{ \
		static const wchar_t* GetCategoryName() { return L#CategoryName; } \
	}

LOG_ADDCATEGORY(Temp);

#define LINE _CRT_WIDE(__LINE__)

#define MR_LOG(CategoryName, Severity, Message, ...) \
	static_assert(!std::is_same<decltype(_exception::retval), const wchar_t*>::value, "Formatting must be either TEXT() or L'Text'"); \
    static_assert(std::is_base_of<SLogEntry, CategoryName>::value, "Category must inherit from SLogEntry (Use LOG_ADDCATEGORY() macro)"); \
    do { \
        Logger::Get().logMessage(LogPart(CategoryName::GetCategoryName(), Severity, Message, __FILEW__, __FUNCTIONW__, __VA_ARGS__), __FUNCTIONW__ , __FILEW__); \
    } while (0)

#ifdef MR_DEBUG
#define MR_ASSERT(expression, message, ...) \
    do { \
        if (!(expression)) \
        {                                                         \
            Logger::Get().logAssert(__FUNCTIONW__, __FILEW__, message, __VA_ARGS__); \
            abort();                                                    \
        }                                                                      \
    } while (0)

#ifdef _WIN32
#endif // _WIN32
#else
#define MR_ASSERT(...) //* This function does nothing, if you want someting, switch to Debug!
#endif // DEBUG




#define CONCATTYPENAME(x) \
    L#x