/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include "LogAssertion.h"
#include <type_traits>

#define MERGE(x, y) x##y
#define MERGE2(x) #x

/** Creates a logging category, do NOT insert Log before, just the word (automatically appends Log)! */
#define LOG_ADDCATEGORY(CategoryName) \
    /* Generated log name! */ \
    struct Log##CategoryName : public LogEntry \
	{ \
        static constexpr const char* GetName() { return MERGE2(Log##CategoryName); }; \
    }

LOG_ADDCATEGORY(Temp);

#define LINE _CRT_WIDE(__LINE__)

#define MR_LOG(CategoryName, Severity, Message, ...) \
	/*static_assert(!std::is_same<decltype(_exception::retval), const wchar_t*>::value, "Formatting must be either TEXT() or L'Text'"); */\
    static_assert(std::is_base_of<LogEntry, CategoryName>::value, "Category must inherit from LogEntry (Use LOG_ADDCATEGORY() macro)"); \
    if constexpr (bIsRunningDebugMode || Severity != Fatal) \
    {\
        do { \
            \
                LogDescriptor Log##Line; Log##Line.SetValues(#CategoryName, Severity, __FUNCTION__, __FILE__, __LINE__); ILogger::Get()->TransmitMessage(&Log##Line); \
                if constexpr (Severity == Fatal) \
                { \
                    ILogger::Get()->HandleFatal(); \
                } \
        } while (0); \
    }

#ifdef MR_DEBUG
#define MR_ASSERT(expression, message, ...) \
    do { \
        if (!(expression)) \
        {   \
            LogAssertion asserta(__FILE__, __LINE__, #expression, message, __VA_ARGS__);\
            if (/*ILogger::Get()->TransmitAssertion(&asserta) == 1*/0) __debugbreak(); \
                                                             \
        }                                                                      \
    } while (0)

#ifdef _WIN64
#endif // _WIN64
#else
#define MR_ASSERT(...) //* This function does nothing, if you want someting, switch to Debug!
#endif // DEBUG




#define CONCATTYPENAME(x) \
    L#x