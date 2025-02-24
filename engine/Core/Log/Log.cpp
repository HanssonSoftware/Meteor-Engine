/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Log.h"
#include <Types/String.h>
//#include <fcntl.h>
//#include <iostream>
#include "LogMacros.h"
#include <Common/MemoryManager.h>
#include <Application/Application.h>
#include "Version.h"
#include <Application/Commandlet.h>
#include <mutex>


#pragma warning(disable : 4700)

#ifdef _WIN32
#pragma comment(lib, "kernel32.lib")
#endif // _WIN32

LOG_ADDCATEGORY(Windows);

Logger& Logger::Get()
{
    static std::mutex mtx;
    std::lock_guard<std::mutex> l(mtx);
    static Logger instance;
    return instance;
}

void Logger::firstStartLogger()
{
    if (ICommandlet::Get().Expected<bool>("nofilelogging") || Application::Get()->getAppInfo()->flags & APPFLAG_NO_FILE_LOGGING)
    {
        loggingState |= LOGGING_LEVEL_NO_FILE_LOGGING;
    }

    if (ICommandlet::Get().Expected<bool>("verbose") || Application::Get()->getAppInfo()->flags & APPFLAG_ENABLE_VERBOSE_LOGGING)
    {
        loggingState |= LOGGING_LEVEL_VERBOSE_LOGGING;
    }

    if (!(loggingState & LOGGING_LEVEL_NO_FILE_LOGGING))
    {
        const String appName = String::Format("%s.txt", Application::Get()->getAppInfo()->appName.Chr());

        stream.open(appName.Chr(), std::ios::app);
        stream << "Logging started at " << Timer::Now().Chr() << "\n";
        stream.flush();
    }

    printCollectedLogs();

#if defined(_WIN32) && MR_DEBUG
    createConsoleWindow();
#else defined(_WIN32)
    FreeConsole();
#endif //
}

void Logger::shutdownLogger()
{
    if (!(loggingState & LOGGING_LEVEL_NO_FILE_LOGGING))
    {
        stream << "Logging closed at " << Timer::Now().Chr() << "\n";
        stream.flush();
        stream.close();
    }
}

void Logger::printCollectedLogs()
{
    if (critialLogs.size() == 0)
        return;

    MR_LOG(LogApplication, Log, TEXT("Broadcasting Collected Logs."));

    for (const LogPart& Log : critialLogs)
    {
        logMessage(Log, Log.func.Chr(), Log.file.Chr());
    }
}

inline void Logger::logMessage(LogPart Log, const wchar_t* Function, const wchar_t* File)
{
    if (Log.displayTitle == Verbose)
    {
        if (!(loggingState & LOGGING_LEVEL_VERBOSE_LOGGING))
            return;
    }

    if (!Application::Get())
    {
        critialLogs.push_back(Log);
        return;
    }

    setColorBySeverity(Log.displayTitle);

    if (Log.displayTitle > 3)
    {
        if (Window* temp = Application::Get()->getWindowManager()->getFocusedWindow())
            temp->drawAttention();
    }

    if (Log.displayTitle == Fatal)
    {
        writeToOutput(formatQuickFatal(Log.rawMessage.Chr(), Function, File).Chr(), true);
        MessageBox(NULL, Log.rawMessage.Chr(), L"Fatal Error!", MB_ICONERROR | MB_OK);

        exit(-1);
    }

    writeToOutput(Log.message.Chr(), (Log.displayTitle == Info) ? false : true);
    setColorBySeverity(ESeverity::Log);
}

void Logger::logAssert(const wchar_t* Function, const wchar_t* File, const wchar_t* Input = L"", ...)
{
    va_list args;
    va_start(args, Input);
    const uint32 requiredChars = vswprintf(NULL, 0, Input, args);
    va_end(args);    
    
    wchar_t* variadicBuffer = new wchar_t[requiredChars + 1];

    va_list argsA;
    va_start(argsA, Input);
    vswprintf_s(variadicBuffer, requiredChars + 1, Input, argsA);
    va_end(argsA);
    

    const uint32 requiredCharsForFullText = swprintf(NULL, 
        0, 
        L"=============[ Assertion error ]=============\nWhere: \t\t%s\nWhen: \t\t%s\nMessage: \t%s\n\nFile: \t%s\n", 
        Function, 
        Timer::Now("%Y-%m-%d %H:%M:%S").Chr(),
        Input,
        File
    );

    wchar_t* fullBuffer = new wchar_t[requiredCharsForFullText + 1];

    swprintf(fullBuffer,
        requiredCharsForFullText,
        L"=============[ Assertion error ]=============\nWhere: \t\t%s\nWhen: \t\t%s\nMessage: \t%s\n\nFile: \t%s\n",
        Function,
        Timer::Now("%Y-%m-%d %H:%M:%S").Chr(),
        variadicBuffer,
        File
    );

    if (Application::Get())
        Application::Get()->getWindowManager()->getFirstWindow()->drawAttention();

    setColorBySeverity(Error);
    writeToOutput(fullBuffer, true);
    setColorBySeverity(Log);

    delete[] fullBuffer;
    delete[] variadicBuffer;
}

void Logger::writeToOutput(const wchar_t* Input, bool bFiled = false)
{
#ifdef _WIN32
    OutputDebugString(Input);
#endif // _WIN32

    wprintf(L"%s", Input);
   
    if (!(loggingState & LOGGING_LEVEL_NO_FILE_LOGGING))
    {
        if (stream.is_open() && bFiled) 
        {
            stream << Input;
            stream.flush();
        }
    }
}

String Logger::getLastError() const noexcept
{
#ifdef _WIN32
    if (GetLastError() == S_OK)
        return String();

    wchar_t* Temp;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        0,
        (LPWSTR)&Temp,
        0, 
        NULL
    );

    return String(Temp);
#else
    // THROW_EXCEPTION("This Function is Not Implemented or Defined in Other Platform!");
    return String("Not Implemented!");
#endif // _WIN32
}

Logger::~Logger()
{
    if (stream.is_open() && !(loggingState & LOGGING_LEVEL_NO_FILE_LOGGING))
    {
        stream << "Logging closed at " << Timer::Now().Chr() << "\n";
        stream.flush();
        stream.close();
    }
}

String Logger::formatQuickFatal(const wchar_t* message, const wchar_t* Callstack, const wchar_t* File) const
{
    const uint32 requiredSize = swprintf(NULL, 0, 
        L"=============[ Critical error ]=============\nWhere:\t\t%s\nWhen:\t\t%s\nMessage:\t%s\n\nFile:\t%s\n"
        , Callstack, Timer::Now("%Y-%m-%d %H:%M:%S").Chr(), message, File);

    wchar_t* newBuffer = new wchar_t[requiredSize + 1];
    
    swprintf(newBuffer, requiredSize + 1,
        L"=============[ Critical error ]=============\nWhere:\t\t%s\nWhen:\t\t%s\nMessage:\t%s\n\nFile:\t%s\n"
        , Callstack, Timer::Now("%Y-%m-%d %H:%M:%S").Chr(), message, File);

    const String super(newBuffer);

    delete[] newBuffer;
    return super;
}

#ifdef _WIN32
void Logger::createConsoleWindow()
{
   AllocConsole();
   FILE* A;
   freopen_s(&A,"CONOUT$", "w", stdout);

   //const int j = _setmode(_fileno(A), _O_U8TEXT);
   const String message = String::Format(L"MR console b%d %s", BUILD_NUMBER, BUILD_DATE);

   CONSOLE_CURSOR_INFO ci;
   GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
   ci.bVisible = 0;
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);

   SetConsoleOutputCP(CP_UTF8);
   SetConsoleTitle(message.Chr());
}
#endif

inline constexpr const void Logger::setColorBySeverity(ESeverity Severity) const noexcept
{
    switch (Severity)
    {
    case Info:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xA /*A*/);
        break;
    case Log:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
        break;
    case Warn:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x6);
        break;
    case Error:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
        break;
    case Fatal:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
        break;
    case Verbose:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
        break;
    }
}

LogPart::LogPart(String Category, ESeverity DisplayTitle, String Message, String location, String function, ...)
    : category(Category), displayTitle(DisplayTitle), message(Message), file(location), func(function)
{
    if (displayTitle == Verbose)
    {
        //if (!(Logger::Get().getLoggingLevel() & LOGGING_LEVEL_NO_FILE_LOGGING))
        //    return;
    }

    time = Timer::Now("%Y-%m-%d %H:%M:%S");

    va_list variadicCount;
    va_start(variadicCount, function);
    const uint32 needForVariadic = vswprintf(NULL, 0, Message.Chr(), variadicCount);
    va_end(variadicCount);

    wchar_t* variadicBuffer = new wchar_t[needForVariadic + 1];

    va_list args;
    va_start(args, function);
    vswprintf(variadicBuffer, needForVariadic + 1, Message.Chr(), args);
    va_end(args);

    rawMessage = variadicBuffer;

    const uint32 needSuper = swprintf(NULL, 0, L"[%s] %s: %s: %s\n", time.Chr(), category.Chr(), formatSeverity(DisplayTitle), variadicBuffer);

    wchar_t* messageBlock = new wchar_t[needSuper + needForVariadic + 1];
    swprintf(messageBlock, needSuper + 1, L"[%s] %s: %s: %s\n", time.Chr(), category.Chr(), formatSeverity(DisplayTitle), variadicBuffer);

    message = messageBlock;

    delete[] variadicBuffer;
    delete[] messageBlock;
}

String getLastError() noexcept
{
    return Logger::Get().getLastError();
}

constexpr const wchar_t* LogPart::formatSeverity(ESeverity Severity) const noexcept
{
    switch (Severity)
    {
    case Info:
        return L"Info";
    case Log:
        return L"Log";
    case Warn:
        return L"Warn";
    case Error:
        return L"Error";
    case Fatal:
        return L"Fatal";
    case Verbose:
        return L"Verbose";
    }

    return L"???";
}
