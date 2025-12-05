/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <type_traits>

#define MERGE(x, y) x##y
#define MERGE2(x) #x


enum LogSeverity : uint8_t
{
    Log,
    Verbose,
    Warn,
    Error,
    Fatal
};

struct LogDescriptor
{
    LogDescriptor() = default;

    constexpr void SetValues(const wchar_t* category, int severity, const wchar_t* function, const wchar_t* file, const int line) noexcept
    {
        this->team = category;
        this->severity = severity;
        this->function = function;
        this->line = line;
        this->file = file;
    }

    void SetMessage(const wchar_t* message, ...);

    int severity;

    int line;

    const wchar_t* team;

    const wchar_t* function;

    const wchar_t* file;

    String message;
};


/** Creates a logging category, do NOT insert Log before, just the word (automatically appends Log)! */
#define LOG_ADDCATEGORY(CategoryName) \
    /* Generated log name! */ \
    struct Log##CategoryName : public LogEntry \
	{ \
        static constexpr const char* GetName() { return #CategoryName; }; \
    }

LOG_ADDCATEGORY(Temp);

#define LINE _CRT_WIDE(__LINE__)
#define Lize(x) L##x

#define MR_LOG(CategoryName, Severity, Message, ...) \
	/*static_assert(!std::is_same<decltype(_exception::retval), const wchar_t*>::value, "Formatting must be either TEXT() or L'Text'"); */\
    static_assert(std::is_base_of<LogEntry, CategoryName>::value, "Category must inherit from LogEntry (Use LOG_ADDCATEGORY() macro)"); \
    if constexpr (bIsRunningDebugMode || Severity != Fatal) \
    {\
        do { \
            \
                LogDescriptor Log##LINE; Log##LINE.SetValues(Lize(#CategoryName), Severity, __FUNCTIONW__, __FILEW__, __LINE__);Log##LINE.SetMessage(Lize(Message), __VA_ARGS__); ILogger::Get()->ProcessMessage(&Log##LINE); \
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
            /*LogAssertion asserta(__FILE__, __LINE__, #expression, message, __VA_ARGS__);*/\
            if (/*ILogger::Get()->TransmitAssertion(&asserta) == 1*/1) __debugbreak(); \
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