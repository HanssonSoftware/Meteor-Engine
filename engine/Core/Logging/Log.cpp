/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Log.h"
#include <Commandlet.h>

#include <Platform/File.h>
#include <Platform/Paths.h>
#include <Platform/PlatformLayout.h>
#include <Platform/Timer.h>
#include <Platform/FileManager.h>

#ifdef MR_PLATFORM_WINDOWS
#include <Windows/WindowsLog.h>
#else

#endif // MR_PLATFORM_WINDOWS

//#include <Core/Application.h>
//#include <Layers/SystemLayer.h>
//#include "LogAssertion.h"

LOG_ADDCATEGORY(Standard);


ILogger::ILogger()
{

}

ILogger* ILogger::Get()
{
#ifdef MR_PLATFORM_WINDOWS
    static WindowsLogger instance;
#endif // MR_PLATFORM_WINDOWS

    return &instance;
}

void ILogger::Shutdown()
{
    if (bIsUsingFile && buffer)
    {
        const String formatted = String::Format("Logging closed at %ls.\n", *Timer::Format("yyyy-MM-dd HH:mm:ss"));

        buffer->Write(&formatted);
        buffer->Close();
    }
}

void ILogger::Initialize()
{
    bIsUsingVerbose = Commandlet::Parse("-verbose", nullptr);

    bIsUsingFile = !Commandlet::Parse("-nofilelogging", nullptr);

    if (bIsUsingFile)
    {
        auto b = Paths::GetExecutableDirctory();
        //buffer = FileManager::CreateFileOperation(
        //    String::Format(
        //        "E:\\Logging\\%ls-%ls.txt", 
        //        app->GetAppInfo().appName.Chr(), 
        //        ITimer::Now("%H.%M.%ls").Chr()),

        //    OPENMODE_WRITE | OPENMODE_READ, SHAREMODE_READ | SHAREMODE_WRITE, OVERRIDERULE_CREATE_NEW_DONT_MIND, 
        //    stat
        //);

        if (buffer) buffer->Write(nullptr);
    }
}

ILogger::~ILogger() noexcept
{
    Shutdown();
}

void ILogger::HandleFatal()
{
    abort();
}

void ILogger::SetActualLog(LogDescriptor* newDescriptor)
{
    actualDescriptor = newDescriptor;
}

void ILogger::TransmitMessage(LogDescriptor* Descriptor)
{
    SetActualLog(Descriptor);

    String fullMessage;
    const String current = Timer::Format("yyyy-MM-dd HH:mm:ss");

    if (Descriptor->severity == Fatal)
    {
        fullMessage = String::Format(
            "=============[ Fatal error ]=============\nWhere:\t\t%ls\nWhen:\t\t%ls\nMessage:\t%ls\n\nFile:\t%ls\n",
            Descriptor->function,
            *current,
            *Descriptor->message,
            Descriptor->file);
    }
    else
    {
        fullMessage = String::Format("[%ls] %ls: %ls\n", 
            current.Chr(),
            Descriptor->team,
            *Descriptor->message);
    }

    SendToOutputBuffer(fullMessage);
}

void ILogger::TransmitAssertion(const LogAssertion* Info)
{
	//if (!Info) return;

 //   MessageBoxDescriptor mbxInfo = {};
 //   mbxInfo.Description = String::Format(
 //       "Assertion failed: %ls\tLine: %d\tFile: %ls\n",
 //       Info->assertStatement,
 //       Info->assertLineInFile,
 //       Info->assertLocationInFile
 //   );
 //   mbxInfo.Title = "Assertion failed";

    //systemLayer->AddMessageBox(&mbxInfo);
}

bool ILogger::IsDebuggerAttached()
{
    return false;
}

void ILogger::SendToOutputBuffer(const String& Buffer)
{
    if (!Buffer.IsEmpty() && buffer)
    {
        buffer->Write(&Buffer);
    }
}

static constexpr const char* FormatSeverity(LogSeverity Severity) noexcept
{
    switch (Severity)
    {
    case Log:
        return "Log";
    case Warn:
        return "Warning";
    case Error:
        return "Error";
    case Fatal:
        return "Fatal";
    case Verbose:
        return "Verbose";
    }

    return "???";
}

void LogDescriptor::SetMessage(const wchar_t* message, ...)
{
    va_list d = nullptr;
    va_start(d, message);
    const int32_t reqAmount = vswprintf(nullptr, 0, message, d);

    wchar_t* big = MemoryManager::Get().Allocate<wchar_t>(reqAmount + 1);
    vswprintf(big, reqAmount + 1, message, d);
    va_end(d);

    big[reqAmount] = L'\0';

    this->message = big;
    MemoryManager::Get().Deallocate(big);
}
