/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Log.h"
#include "LogMacros.h"
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
    if (instance->bIsUsingFile && instance->buffer)
    {
        const Time current = Timer::Now();

        instance->buffer->Write(String::Format("Logging closed at %d-%d-%d %d:%d.\n", current.year, current.month, current.day, current.hour, current.minute));
        instance->buffer->Close();
    }
}

void ILogger::Initialize()
{
    String val;
    instance->bIsUsingVerbose = Commandlet::Parse("-verbose", val);

    instance->bIsUsingFile = !Commandlet::Parse("-nofilelogging", val);

    if (instance->bIsUsingFile && false)
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
    instance->actualDescriptor = newDescriptor;
}

void ILogger::TransmitMessage(LogDescriptor* Descriptor)
{
    SetActualLog(Descriptor);

    String fullMessage;
    Time current = Timer::Now();

    if (Descriptor->severity == Fatal)
    {
        fullMessage = String::Format(
            "=============[ Fatal error ]=============\nWhere:\t\t%s\nWhen:\t\t%d-%d-%d %d:%d\nMessage:\t%s\n\nFile:\t%s\n",
            Descriptor->function,
            current.year, current.month, current.day, current.hour, current.minute,
            Descriptor->message.Chr(),
            Descriptor->file
        );
    }
    else
    {
        fullMessage = String::Format("[%d-%d-%d %d:%d] %s: %s\n", 
            current.year, current.month, current.day, current.hour, current.minute,
            Descriptor->team, 
            *Descriptor->message
        );
    }
    

    instance->SendToOutputBuffer(fullMessage);
}

void ILogger::TransmitAssertion(const LogAssertion* Info)
{
	if (!Info) return;

    if (SystemLayer* systemLayer = Layer::GetSystemLayer())
    {
        MessageBoxDescriptor mbxInfo = {};
        mbxInfo.Description = String::Format(
            "Assertion failed: %s\tLine: %d\tFile: %s\n",
            Info->assertStatement,
            Info->assertLineInFile,
            Info->assertLocationInFile
        );
        mbxInfo.Title = "Assertion failed";

        systemLayer->AddMessageBox(&mbxInfo);
    }
}

bool ILogger::IsDebuggerAttached()
{
    return false;
}

void ILogger::SendToOutputBuffer(const String& Buffer)
{
    if (!Buffer.IsEmpty() && instance->buffer)
    {
        instance->buffer->Write(Buffer);
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
