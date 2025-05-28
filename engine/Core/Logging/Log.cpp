/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Log.h"
#include "LogMacros.h"
#include <Commandlet.h>
#include <Core/Application.h>
#include <Platform/Timer.h>
#include <Platform/File.h>
#include <Layers/OSLayer.h>
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

}

//
//void ILogger::firstStartLogger()
//{
//    if (ICommandlet::Get().Expected<bool>("nofilelogging") || Application::Get()->appInfo.flags & APPFLAG_NO_FILE_LOGGING)
//    {
//        loggingState |= LOGGING_LEVEL_NO_FILE_LOGGING;
//    }
//
//    if (ICommandlet::Get().Expected<bool>("verbose") || Application::Get()->appInfo.flags & APPFLAG_ENABLE_VERBOSE_LOGGING)
//    {
//        loggingState |= LOGGING_LEVEL_VERBOSE_LOGGING;
//    }
//
//    if (!(loggingState & LOGGING_LEVEL_NO_FILE_LOGGING))
//    {
//        const String appName = String::Format("%s.txt", Application::Get()->appInfo.appName.Chr());
//
//        stream.open(appName.Chr(), std::ios::app);
//        stream << "Logging started at " << Timer::Now().Chr() << "\n";
//        stream.flush();
//    }
//
//    SendPreInitLogs();
//
//#if defined(_WIN64) && MR_DEBUG
//    createConsoleWindow();
//#else defined(_WIN64)
//    FreeConsole();
//#endif //
//}

//void ILogger::shutdownLogger()
//{
    //if (!(loggingState & LOGGING_LEVEL_NO_FILE_LOGGING))
    //{
    //    stream << "Logging closed at " << Timer::Now().Chr() << "\n";
    //    stream.flush();
    //    stream.close();
    //}
//}

void ILogger::Initialize()
{
    if (const Application* app = Application::Get())
    {
		app->GetAppInfo().flags & APPFLAG_ENABLE_VERBOSE_LOGGING ? instance->bIsUsingVerbose = true : instance->bIsUsingVerbose = false;
		app->GetAppInfo().flags & APPFLAG_NO_FILE_LOGGING ? instance->bIsUsingFile = false : instance->bIsUsingFile = true;
	}

    if (!instance->bIsUsingVerbose) instance->bIsUsingVerbose = ICommandlet::Get().Expected<bool>("verbose");

    //if (bIsUsingFile) bIsUsingFile = !ICommandlet::Get().Expected<bool>("nofilelogging");

    if (instance->bIsUsingFile)
    {
        if (const Application* app = Application::Get())
        {
            FileStatus stat;
            instance->buffer = FileManager::CreateFileOperation(String::Format("E:\\Logging\\%s-%s.txt", app->GetAppInfo().appName.Chr(), Timer::Now("%H.%M.%S").Chr()), OPENMODE_WRITE | OPENMODE_READ, SHAREMODE_READ | SHAREMODE_WRITE, OVERRIDERULE_CREATE_NEW_DONT_MIND, stat);

            instance->buffer->Write(String::Format("Logging started at %s.", Timer::Now("%Y/%m/%d-%H:%M:%S").Chr()));
        }
    }
}

ILogger::~ILogger() noexcept
{
    if (bIsUsingFile && buffer)
    {
        buffer->Write(String::Format("Logging closed at %s.\n", Timer::Now().Chr()));
        buffer->Close();
    }
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

    if (OSLayer* systemLayer = Layer::GetSystemLayer())
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

//inline void ILogger::logMessage(LogPart Log, const char* Function, const char* File)
//{
//    if (Log.displayTitle == LOG_SEVERITY_VERBOSE)
//    {
//        if (!(loggingState & LOGGING_LEVEL_VERBOSE_LOGGING))
//            return;
//    }
//
//    if (!Application::Get())
//    {
//        critialLogs.push_back(Log);
//        return;
//    }
//
//    setColorBySeverity(Log.displayTitle);
//
//    //if (Log.displayTitle > 3)
//    //{
//    //    if (IWindow* temp = Application::Get()->GetWindowManager()->GetFocusedWindow())
//    //        temp->DrawAttention();
//    //}
//
//    if (Log.displayTitle == LOG_SEVERITY_FATAL)
//    {
//        writeToOutput(formatQuickFatal(Log.rawMessage.Chr(), Function, File).Data(), true);
//#ifdef _WIN64
//        //const uint32 size = String::Wide(Log.rawMessage, nullptr);
//        //wchar_t* super = new wchar_t[size + 1];
//        //String::Wide(Log.rawMessage, super);
//
//        //HANDLE appMtx = CreateMutex(0, 1, super);
//
//        //MessageBox(NULL, super, L"Fatal Error!", MB_ICONERROR | MB_OK);
//        //delete[] super;
//
//#endif // _WIN64
//        exit(-1);
//    }
//
//    writeToOutput(Log.message.Data(), (Log.displayTitle == LOG_SEVERITY_INFO) ? false : true);
//    setColorBySeverity(LogSeverity::LOG_SEVERITY_LOG);
//}
//
//void ILogger::logAssert(const char* Function, const char* File, const char* Input = "", ...)
//{
//    va_list args;
//    va_start(args, Input);
//    const uint32 requiredChars = vsprintf(0, Input, args);
//    va_end(args);    
//    
//    char* variadicBuffer = new char[requiredChars + 1];
//
//    va_list argsA;
//    va_start(argsA, Input);
//    vsprintf(variadicBuffer, Input, argsA);
//    va_end(argsA);
//    
//
//    const uint32 requiredCharsForFullText = sprintf(
//        0, 
//        "=============[ Assertion error ]=============\nWhere: \t\t%s\nWhen: \t\t%s\nMessage: \t%s\n\nFile: \t%s\n", 
//        Function, 
//        Timer::Now("%Y-%m-%d %H:%M:%S").Chr(),
//        Input,
//        File
//    );
//
//    char* fullBuffer = new char[requiredCharsForFullText + 1];
//
//    sprintf(
//        fullBuffer,
//        "=============[ Assertion error ]=============\nWhere: \t\t%s\nWhen: \t\t%s\nMessage: \t%s\n\nFile: \t%s\n",
//        Function,
//        Timer::Now("%Y-%m-%d %H:%M:%S").Chr(),
//        variadicBuffer,
//        File
//    );
//
//    if (Application::Get()) Application::Get()->GetWindowManager()->GetFirstWindow()->DrawAttention();
//
//    setColorBySeverity(LOG_SEVERITY_ERROR);
//    writeToOutput(fullBuffer, true);
//    setColorBySeverity(LOG_SEVERITY_LOG);
//
//    delete[] fullBuffer;
//    delete[] variadicBuffer;
//}
//
//void ILogger::writeToOutput(char* Input, bool bFiled = false)
//{
//#ifdef _WIN64
//    if (OSLayer* layer = Layer::GetSystemLayer())
//    {
//        const wchar_t* bf = layer->ConvertToWide(Input);
//        OutputDebugStringW(bf);
//
//
//        delete[] bf;
//    }
//#endif // _WIN64
//
//    const size_t length = strlen(Input);
//    if (Input[length - 2] == '\n' && Input[length - 1] == '\n')
//        Input[length - 1] = '\0';
//
//    printf("%s", Input);
//   
//    if (!(loggingState & LOGGING_LEVEL_NO_FILE_LOGGING))
//    {
//        if (stream.is_open() && bFiled) 
//        {
//            stream << Input;
//            stream.flush();
//        }
//    }
//}
//
//String ILogger::formatUnimplemented(const LogPart& Log, const char* dummy, ...)
//{
//    va_list args;
//    va_start(args, dummy);
//    const uint32 requiredChars = vsprintf(0, Log.rawMessage.Chr(), args);
//    va_end(args);
//
//    char* variadicBuffer = new char[requiredChars + 1];
//
//    va_list argsA;
//    va_start(argsA, dummy);
//    vsprintf(variadicBuffer, Log.rawMessage.Chr(), argsA);
//    va_end(argsA);
//
//
//    const uint32 requiredCharsForFullText = sprintf(
//        0,
//        "=============[ Unimplemented Function ]=============\nWhere: \t\t%s\n\nFile: \t%s\n",
//        Log.func.Chr(),
//        Log.file.Chr()
//    );
//
//    char* fullBuffer = new char[requiredCharsForFullText + 1];
//
//    sprintf(
//        fullBuffer,
//        "=============[ Unimplemented Function ]=============\nWhere: \t\t%s\n\nFile: \t%s\n",
//        Log.func.Chr(),
//        Log.file.Chr()
//    );
//
//    const String f(fullBuffer);
//
//    delete[] fullBuffer;
//    return f;
//}
//
//ILogger::~ILogger()
//{
//    if (stream.is_open() && !(loggingState & LOGGING_LEVEL_NO_FILE_LOGGING))
//    {
//        stream << "Logging closed at " << Timer::Now().Chr() << "\n";
//        stream.flush();
//        stream.close();
//    }
//}
//
//String ILogger::formatQuickFatal(const char* message, const char* Callstack, const char* File) const
//{
//    const uint32 requiredSize = sprintf(0, 
//        "=============[ Critical error ]=============\nWhere:\t\t%s\nWhen:\t\t%s\nMessage:\t%s\n\nFile:\t%s\n"
//        , Callstack, Timer::Now("%Y-%m-%d %H:%M:%S").Chr(), message, File);
//
//    char* newBuffer = new char[requiredSize + 1];
//    
//    sprintf(newBuffer,
//        "=============[ Critical error ]=============\nWhere:\t\t%s\nWhen:\t\t%s\nMessage:\t%s\n\nFile:\t%s\n"
//        , Callstack, Timer::Now("%Y-%m-%d %H:%M:%S").Chr(), message, File);
//
//    const String super(newBuffer);
//
//    delete[] newBuffer;
//    return super;
//}
//
//#ifdef _WIN64
//void ILogger::createConsoleWindow()
//{
//   AllocConsole();
//   FILE* A;
//   freopen_s(&A,"CONOUT$", "w", stdout);
//
//   //const int j = _setmode(_fileno(A), _O_U8TEXT);
//   const String message = String::Format(L"MR console b%d %s", BUILD_NUMBER, BUILD_DATE);
//
//   CONSOLE_CURSOR_INFO ci;
//   GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
//
//   ci.bVisible = 0;
//   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
//
//   SetConsoleOutputCP(CP_UTF8);
//   SetConsoleTitleA(message.Chr());
//}
//#endif
//
//inline constexpr const void ILogger::setColorBySeverity(LogSeverity Severity) const noexcept
//{
//    switch (Severity)
//    {
//    case LOG_SEVERITY_INFO:
//        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xA /*A*/);
//        break;
//    case LOG_SEVERITY_LOG:
//        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
//        break;
//    case LOG_SEVERITY_WARN:
//        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x6);
//        break;
//    case LOG_SEVERITY_ERROR:
//        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
//        break;
//    case LOG_SEVERITY_FATAL:
//        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
//        break;
//    case LOG_SEVERITY_VERBOSE:
//        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xB);
//        break;
//    }
//}
//
//LogPart::LogPart(String Category, LogSeverity DisplayTitle, String Message, String location, String function, ...)
//    : category(Category), displayTitle(DisplayTitle), message(Message), file(location), func(function)
//{
//    if (displayTitle == LOG_SEVERITY_VERBOSE)
//    {
//        //if (!(Logger::Get().getLoggingLevel() & LOGGING_LEVEL_NO_FILE_LOGGING))
//        //    return;
//    }
//
//    time = Timer::Now("%Y-%m-%d %H:%M:%S");
//
//    va_list variadicCount;
//    va_start(variadicCount, function);
//    const uint32 needForVariadic = vsnprintf(0, 0, Message.Chr(), variadicCount);
//    va_end(variadicCount);
//
//    char* variadicBuffer = new char[needForVariadic + 1];
//
//    va_list args;
//    va_start(args, function);
//    vsnprintf(variadicBuffer, needForVariadic + 1,Message.Chr(), args);
//    va_end(args);
//
//    rawMessage = variadicBuffer;
//
//    const uint32 needSuper = snprintf(0, 0, "[%s] %s: %s: %s\n", time.Chr(), category.Chr(), formatSeverity(DisplayTitle), variadicBuffer);
//
//    char* messageBlock = new char[needSuper + needForVariadic + 1];
//    snprintf(messageBlock, needSuper + needForVariadic + 1,"[%s] %s: %s: %s\n", time.Chr(), category.Chr(), formatSeverity(DisplayTitle), variadicBuffer);
//
//    message = messageBlock;
//
//    delete[] variadicBuffer;
//    delete[] messageBlock;
//}
//
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
