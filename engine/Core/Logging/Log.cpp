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
        instance->buffer->Write(String::Format("Logging closed at %s.\n", Timer::Now().Chr()));
        instance->buffer->Close();
    }
}

void ILogger::Initialize()
{
    if (const Application* app = GetApplication())
    {
		app->GetAppInfo().flags & APPFLAG_ENABLE_VERBOSE_LOGGING ? instance->bIsUsingVerbose = true : instance->bIsUsingVerbose = false;
		app->GetAppInfo().flags & APPFLAG_NO_FILE_LOGGING ? instance->bIsUsingFile = false : instance->bIsUsingFile = true;
	}

    if (!instance->bIsUsingVerbose) instance->bIsUsingVerbose = ICommandlet::Parse("-verbose").IsEmpty();

    if (instance->bIsUsingFile) instance->bIsUsingFile = !ICommandlet::Parse("-nofilelogging").IsEmpty();

    if (instance->bIsUsingFile)
    {
        if (const Application* app = GetApplication())
        {
            FileStatus stat;
            instance->buffer = FileManager::CreateFileOperation(
                String::Format(
                    "E:\\Logging\\%s-%s.txt", 
                    app->GetAppInfo().appName.Chr(), 
                    Timer::Now("%H.%M.%S").Chr()),

                OPENMODE_WRITE | OPENMODE_READ, SHAREMODE_READ | SHAREMODE_WRITE, OVERRIDERULE_CREATE_NEW_DONT_MIND, 
                stat
            );

            instance->buffer->Write(String::Format("Logging started at %s.\n", Timer::Now("%Y/%m/%d-%H:%M:%S").Chr()));
        }
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
    if (Descriptor->severity == Fatal)
    {
        fullMessage = String::Format(
            "=============[ Fatal error ]=============\nWhere:\t\t%s\nWhen:\t\t%s\nMessage:\t%s\n\nFile:\t%s\n",
            Descriptor->function,
            Timer::Now("%Y-%m-%d %H:%M:%S").Chr(),
            Descriptor->message.Chr(),
            Descriptor->file
        );
    }
    else
    {
        fullMessage = String::Format("[%s] %s: %s\n", Timer::Now().Chr(), Descriptor->team, Descriptor->message.Chr());
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

void ILogger::SendToOutputBuffer(const String Buffer)
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
