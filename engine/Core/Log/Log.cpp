/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Log.h"
//#include <sstream>
#include <fcntl.h>
#include <io.h>
//#include <iostream>
#include "Version.h"
#include "LogMacros.h"
#include <Common/MemoryManager.h>
#include <Application/Application.h>

#pragma warning(disable : 4700)

#ifdef _WIN32
#pragma comment(lib, "kernel32.lib")
#endif // _WIN32

LOG_ADDCATEGORY(Windows);

Logger& Logger::Get()
{
    static Logger instance;
    return instance;
}

void Logger::firstStartLogger()
{
    String appName = Application::Get().getAppInfo()->appName;

    appName = appName + L".txt";

    stream.open(appName.Chr(), std::ios::app);
    stream << "Logging started at " << Timer::Now().Chr() << "\n";
    stream.flush();

#if defined(_WIN32) && MR_DEBUG
    createConsoleW();
#else defined(_WIN32)
    FreeConsole();
#endif //
}

void Logger::shutdownLogger()
{
    stream << "Logging closed at " << Timer::Now().Chr() << "\n";
    stream.flush();
    stream.close();
}

void Logger::printCollectedLogs()
{
    for (LogPart Log : critialLogs)
    {

    }
}

void Logger::logMessage(LogPart Log, const wchar_t* Function, const wchar_t* File)
{
    //if (Application::Get())

    setColorBySeverity(Log.displayTitle);

    if (Log.displayTitle > 3)
        Application::Get().drawAttention();

    if (Log.displayTitle == Fatal)
    {
        writeToOutput(formatQuickFatal(Log.message.Chr(), Function, File).Chr(), true);
        //LogMessage(Log, L"LogWindows", nullptr, nullptr, L"Sending MessageBox to the user");
        MessageBox(NULL, Log.rawMessage.Chr(), L"Catastrophic Error!", MB_ICONERROR | MB_OK);

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
    
    wchar_t* variadicBuffer = (wchar_t*)mrmalloc((requiredChars + 1) * sizeof(wchar_t));

    va_list argsA;
    va_start(argsA, Input);
    vswprintf_s(variadicBuffer, requiredChars + 1, Input, argsA);
    va_end(argsA);
    

    const uint32 requiredCharsForFullText = swprintf(NULL, 
        0, 
        L"=============[ Assertion error ]=============\nWhere: \t\t%s\nWhen: \t\t%s\nMessage: \t%s\n\nFile: \t%s\n", 
        Function, 
        Timer::Now().Chr(),
        Input,
        File
    );

    wchar_t* fullBuffer = (wchar_t*)mrmalloc((requiredCharsForFullText + 1) * sizeof(wchar_t));

    swprintf(fullBuffer,
        requiredCharsForFullText,
        L"=============[ Assertion error ]=============\nWhere: \t\t%s\nWhen: \t\t%s\nMessage: \t%s\n\nFile: \t%s\n",
        Function,
        Timer::Now().Chr(),
        variadicBuffer,
        File
    );

    Application::Get().drawAttention();

    setColorBySeverity(Error);
    writeToOutput(fullBuffer, true);
    setColorBySeverity(Log);

    mrfree(fullBuffer);
    mrfree(variadicBuffer);
}

void Logger::writeToOutput(const wchar_t* Input, bool bFiled = false)
{
#ifdef _WIN32
    OutputDebugString(Input);
#endif // _WIN32
    wprintf(L"%s", Input);
   
    if (stream.is_open() && bFiled) {
        //char* Temp[1024];
        stream << Input;
        stream.flush();
    }
}

Logger::~Logger()
{

}

String Logger::formatQuickFatal(const wchar_t* message, const wchar_t* Callstack, const wchar_t* File) const
{
    const uint32 requiredSize = swprintf(NULL, 0, 
        L"=============[ Critical error ]=============\nWhere:\t\t%s\nWhen:\t\t%s\nMessage:\t%s\n\nFile:\t%s\n"
        , Callstack, Timer::Now().Chr(), message, File);

    wchar_t* newBuffer = (wchar_t*)mrmalloc((requiredSize + 1) * sizeof(wchar_t));
    
    swprintf(newBuffer, requiredSize + 1,
        L"=============[ Critical error ]=============\nWhere:\t\t%s\nWhen:\t\t%s\nMessage:\t%s\n\nFile:\t%s\n"
        , Callstack, Timer::Now().Chr(), message, File);

    const String super(newBuffer);
    mrfree(newBuffer);
    return super;
}

void Logger::createConsoleW()
{
   AllocConsole();
   FILE* A;
   freopen_s(&A,"CONOUT$", "w", stdout);

   const int j = _setmode(_fileno(A), _O_U8TEXT);
   const String message = String(L"Meteor Engine console (b") + String(BUILD_NUMBER) + L")";

   SetConsoleOutputCP(CP_UTF8);
   SetConsoleTitle(message.Chr());
}

const wchar_t* Logger::dispatchLastError()
{
    if (GetLastError() == S_OK)
        return nullptr;

    wchar_t* Temp;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        0,
        (LPWSTR)&Temp,
        0, NULL);

    MR_LOG(LogWindows, Warn, TEXT("%ls"), (wchar_t*)Temp);
    return nullptr;
}

const wchar_t* Logger::dispatchLastError(HRESULT result)
{
    if (result == S_OK)
        return nullptr;

    wchar_t* Temp;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        result,
        0,
        (LPWSTR)&Temp,
        0, NULL);

    MR_LOG(LogWindows, Warn, TEXT("%ls"), (wchar_t*)Temp);

    //const size_t bufferSize = swprintf(NULL, 0, L"0x%x\tDescription: %ls", GetLastError(), (LPCWSTR)Temp);

    //wchar_t* buffer = (wchar_t*)mrmalloc((bufferSize + 1) * sizeof(wchar_t));

    //swprintf(buffer, bufferSize + 1, L"0x%x\tDescription: %ls", GetLastError(), (LPCWSTR)Temp);
    //String formatted(buffer);

    //mrfree(buffer);
    return nullptr;
}

constexpr const void Logger::setColorBySeverity(ESeverity Severity) const noexcept
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
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x3);
        break;
    }
}

LogPart::LogPart(String Category, ESeverity DisplayTitle, String Message, ...)
    : category(Category), displayTitle(DisplayTitle), message(Message)
{
    time = Timer::Now();

    va_list variadicCount;
    va_start(variadicCount, Message);
    const uint32 needForVariadic = vswprintf(NULL, 0, Message.Chr(), variadicCount);
    va_end(variadicCount);

    wchar_t* variadicBuffer = (wchar_t*)mrmalloc((needForVariadic + 1) * sizeof(wchar_t));
    rawMessage = variadicBuffer;

    va_list args;
    va_start(args, Message);
    vswprintf(variadicBuffer, needForVariadic + 1, Message.Chr(), args);
    va_end(args);

    const uint32 needSuper = swprintf(NULL, 0, L"[%s] %s: %s: %s\n", time.Chr(), category.Chr(), formatSeverity(DisplayTitle), variadicBuffer);

    wchar_t* messageBlock = (wchar_t*)mrmalloc((needSuper + needForVariadic + 1) * sizeof(wchar_t));
    swprintf(messageBlock, needSuper + 1, L"[%s] %s: %s: %s\n", time.Chr(), category.Chr(), formatSeverity(DisplayTitle), variadicBuffer);

    message = messageBlock;

    mrfree(variadicBuffer);
    mrfree(messageBlock);
}

LogPart::LogPart(String Category, ESeverity DisplayTitle, String Message)
{
    time = Timer::Now();

    const uint32 needSuper = swprintf(NULL, 0, L"[%s] %s: %s: %s\n", time.Chr(), category.Chr(), formatSeverity(DisplayTitle), Message.Chr());

    wchar_t* messageBlock = (wchar_t*)mrmalloc((needSuper + 1) * sizeof(wchar_t));
    swprintf(messageBlock, needSuper + 1, L"[%s] %s: %s: %s\n", time.Chr(), category.Chr(), formatSeverity(DisplayTitle), Message.Chr());

    rawMessage = messageBlock;
    message = messageBlock;

    mrfree(messageBlock);
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

constexpr const wchar_t* Logger::getMessageName(unsigned int code) const noexcept
{
    switch (code)
    {
    case WM_CREATE:
        return L"WM_CREATE";
    case WM_DESTROY:
        return L"WM_DESTROY";
    case WM_CLOSE:
        return L"WM_CLOSE";
    case WM_SIZING:
        return L"WM_SIZING";
    case WM_WINDOWPOSCHANGING:
        return L"WM_WINDOWPOSCHANGING";
    case WM_WINDOWPOSCHANGED:
        return L"WM_WINDOWPOSCHANGED";
    default:
        return L"";
    }

    return L"";
}