/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Log.h"
#include <Commandlet.h>
#include <Core/Application.h>
#include <Platform/Timer.h>
#include <Platform/File.h>
#include <Layers/SystemLayer.h>
#include <Platform/FileManager.h>
#include "LogAssertion.h"

LOG_ADDCATEGORY(Standard);

static_assert(!std::is_same_v<Logger, ILogger>, "ILogger must be extended!");

static Logger* instance = new Logger();

ILogger::ILogger()
{

}

ILogger* ILogger::Get()
{
    return instance;
}

void ILogger::Shutdown()
{
    if (bIsUsingFile && buffer)
    {
        const String current = Timer::Format("yyyy-MM-dd HH:mm:ss");

        buffer->Write(String::Format("Logging closed at %s.\n", *current));
        buffer->Close();
    }
}

void ILogger::Initialize()
{
    String val;
    bIsUsingVerbose = Commandlet::Parse("-verbose", val);

    bIsUsingFile = !Commandlet::Parse("-nofilelogging", val);

    if (bIsUsingFile && false)
    {
        //if (const Application* app = GetApplication())
        //{
        //    FileStatus stat;
        //    instance->buffer = FileManager::CreateFileOperation(
        //        String::Format(
        //            "E:\\Logging\\%s-%s.txt", 
        //            app->GetAppInfo().appName.Chr(), 
        //            ITimer::Now("%H.%M.%S").Chr()),

        //        OPENMODE_WRITE | OPENMODE_READ, SHAREMODE_READ | SHAREMODE_WRITE, OVERRIDERULE_CREATE_NEW_DONT_MIND, 
        //        stat
        //    );

        //    if (instance->buffer) instance->buffer->Write(String::Format("Logging started at %s.\n", ITimer::Now("%Y/%m/%d-%H:%M:%S").Chr()));
        //}
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
            Descriptor->message.Chr(),
            Descriptor->file
        );
    }
    else
    {
        fullMessage = String::Format("[%s] %s: %s\n", 
            *current,
            Descriptor->team, 
            *Descriptor->message
        );
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
        buffer->Write(Buffer);
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

LogDescriptor::LogDescriptor(const char* entry, LogSeverity severity, const char* Message, const char* function, const char* file, ...) noexcept
    : team(entry), severity(severity), function(function), file(file)
{
    va_list d = nullptr;
    va_start(d, file);
    const int32_t reqAmount = vsnprintf(nullptr, 0, Message, d);

    char* big = MemoryManager::Get().Allocate<char>(reqAmount + 1);
    vsnprintf(big, reqAmount + 1, Message, d);
    va_end(d);

    message = big;
    MemoryManager::Get().Deallocate(big);
}
