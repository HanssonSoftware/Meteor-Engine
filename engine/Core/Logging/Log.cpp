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
        const String formatted = String::Format("Logging closed at %s.\n", *Timer::Format("yyyy-MM-dd HH:mm:ss"));

        buffer->Write(&formatted);
        buffer->Close();
    }
}

void ILogger::Initialize()
{
    String val;
    bIsUsingVerbose = Commandlet::Parse("-verbose", val);

    bIsUsingFile = !Commandlet::Parse("-nofilelogging", val);

    if (bIsUsingFile)
    {
        auto b = Paths::GetEngineDirectory();
        //buffer = FileManager::CreateFileOperation(
        //    String::Format(
        //        "E:\\Logging\\%s-%s.txt", 
        //        app->GetAppInfo().appName.Chr(), 
        //        ITimer::Now("%H.%M.%S").Chr()),

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
            "=============[ Fatal error ]=============\nWhere:\t\t%s\nWhen:\t\t%s\nMessage:\t%s\n\nFile:\t%s\n",
            Descriptor->function,
            *current,
            Descriptor->message,
            Descriptor->file);
    }
    else
    {
        fullMessage = String::Format("[%s] %s: %s\n", 
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
 //       "Assertion failed: %s\tLine: %d\tFile: %s\n",
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

void LogDescriptor::SetMessage(const char* message, ...)
{
    va_list d = nullptr;
    va_start(d, message);
    const int32_t reqAmount = vsnprintf(nullptr, 0, message, d);

    char* big = MemoryManager::Get().Allocate<char>(reqAmount + 1);
    vsnprintf(big, reqAmount + 1, message, d);
    va_end(d);

    this->message = big;
    MemoryManager::Get().Deallocate(big);
}
