/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "WinLayer.h"

#define _WIN64_WINNT 0x0600
#include "WinAPI.h"
#include <Common/Pointers.h>
#include <Core/Application.h>

LOG_ADDCATEGORY(WindowsLayer);

static MSG messageForProcedure;
static HWND focusedWindow;

WinLayer::WinLayer(const String Name) noexcept
    : OSLayer(Name)
{

}

void WinLayer::Init()
{
    const String version = GetMachineVersion();
    if (strcmp(version.Chr(), "6.1") == 0 || strcmp(version.Chr(), "6.2") == 0)
    {
        MessageBoxDescriptor unsupportedInfo = {};
        unsupportedInfo.Description = "This Windows version is not supported!";
        unsupportedInfo.Title = "Unsupported Windows";
        unsupportedInfo.Type = MESSAGEBOXBUTTONS_OK | MESSAGEBOXTYPE_ERROR;

        AddMessageBox(&unsupportedInfo);
        if (Application* app = Application::Get())
        {
            app->Shutdown();
            return;
        }
    }


    IWindow* Ref = Application::Get()->GetWindowManager()->SearchFor("Super");

    //std::atomic<float> dt = 0.f;

    if (Ref)
        focusedWindow = (HWND)Ref->GetWindowHandle();
}

void WinLayer::Attached()
{

}

void WinLayer::Removed()
{
}

void WinLayer::Update()
{
    if (PeekMessage(&messageForProcedure, focusedWindow, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&messageForProcedure);
        DispatchMessage(&messageForProcedure);
    }
}

String WinLayer::GetError()
{
    DWORD Num = GetLastError();
    LPVOID buffer;

    const DWORD count = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        0,
        Num,
        0,
        (LPTSTR)&buffer,
        0,
        0
    );

    //ConvertToNarrow((LPTSTR)buffer);

    const int requiredSizeInBytes = WideCharToMultiByte(/*CP_UTF8*/ 65001, 0, (LPTSTR)buffer, -1, 0, 0, 0, 0);
    if (requiredSizeInBytes == 0) return "Check!";

    char* super = new char[requiredSizeInBytes];

    WideCharToMultiByte(/*CP_UTF8*/ 65001, 0, (LPTSTR)buffer, -1, super, requiredSizeInBytes, 0, 0);
    String realBuffer(super);
    

    LocalFree(buffer);
    if (super) delete[] super;
    return realBuffer;
}

static String version;
String WinLayer::GetMachineVersion()
{
    if (!version.IsEmpty()) return version;


    HKEY versionKey;
    LSTATUS err = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &versionKey);
    if (err != ERROR_SUCCESS)
    {
        RegCloseKey(versionKey);

        MR_LOG(LogWindowsLayer, Error, "RegOpenKeyExW returned: %s", GetError().Chr());
        return "0";
    }
    
    DWORD size = 0;
    err = RegGetValueW(versionKey, 0, L"LCUVer", RRF_RT_REG_SZ, 0, 0, &size);

    wchar_t* buffer = nullptr;
    if (err == ERROR_FILE_NOT_FOUND)
    {
        err = RegGetValueW(versionKey, 0, L"CurrentVersion", RRF_RT_REG_SZ, 0, 0, &size);

        buffer = new wchar_t[size];
        err = RegGetValueW(versionKey, 0, L"CurrentVersion", RRF_RT_REG_SZ, 0, buffer, &size);
        if (err != ERROR_SUCCESS)
        {
            RegCloseKey(versionKey);

            MR_LOG(LogWindowsLayer, Error, "RegGetValueW returned: %s", GetError().Chr());
            return "";
        }
    }
    else
    {
        buffer = new wchar_t[size];
        err = RegGetValueW(versionKey, 0, L"LCUVer", RRF_RT_REG_SZ, 0, buffer, &size);
        if (err != ERROR_SUCCESS)
        {
            RegCloseKey(versionKey);

            MR_LOG(LogWindowsLayer, Error, "RegGetValueW returned: %s", GetError().Chr());
            return "";
        }
    }


    version = buffer;

    RegCloseKey(versionKey);
    return version;
}

Vector3<float> WinLayer::InspectPixel(const Vector2<float> screenCoordinates)
{
    MR_LOG(LogWindowsLayer, Error, "This function is unimplemented! %s", __FUNCTION__);
    static const Vector3<float> colors(/*GetRValue(winColor), GetGValue(winColor), GetBValue(winColor)*/0);

    //if (ReleaseDC(0, screen) == 0)
    //{
    //    MR_LOG(LogWindowsLayer, Error, "Unable to Release DC!");
    //    return Vector3<float>(colors);
    //}

    return colors;
}

String WinLayer::GetMachineName()
{
    wchar_t pc[128];

    unsigned long name = 128;
    GetComputerNameW(pc, &name);

    return String(pc);
}

bool WinLayer::IsRunningAnAnotherInstance()
{
    if (const Application* app = Application::Get())
    {
        const wchar_t* MrWide = ConvertToWide(app->GetAppInfo().appName);
        HANDLE appMtx = CreateMutex(0, 1, MrWide);

        delete[] MrWide;
        
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            if (appMtx) CloseHandle(appMtx);
            return true;
        }
        
        return false;
    }

    return false;
}

MessageBoxDecision WinLayer::AddMessageBox(const MessageBoxDescriptor* Info)
{
    if (!Info) return MESSAGEBOXDECISION_INVALID;

    HWND windowRef = nullptr;
    if (Info->dependentWindow) windowRef = (HWND)Info->dependentWindow->GetWindowHandle();

    const wchar_t* bufferDesc = ConvertToWide(Info->Description.Chr());
    const wchar_t* bufferTitle = ConvertToWide(Info->Title.Chr());

    const uint32 flags = evaluateMessageBoxFlags(Info->Type);

    const int returnCode = MessageBox(windowRef, bufferDesc, bufferTitle, flags);
    delete[] bufferDesc, bufferTitle;
    if (returnCode == 0)
    {
        MR_LOG(LogWindowsLayer, Error, "MessageBox returned: %s", GetError().Chr());
        return MESSAGEBOXDECISION_INVALID;
    }


    return evaluateMessageBoxReturn(returnCode);
}

wchar_t* WinLayer::ConvertToWide(const char* Buffer)
{
    if (!Buffer)
    {
        MR_LOG(LogWindowsLayer, Error, "Attempted to convert an invalid buffer!");
        return L'';
    }

    const int requiredSize = MultiByteToWideChar(65001 /*CP_UTF8*/, 0, Buffer, -1, 0, 0);
    if (requiredSize == 0)
    {
        MR_LOG(LogWindowsLayer, Error, "MultiByteToWideChar said: %s", GetError().Chr());
        return L'';
    }

    wchar_t* super = new wchar_t[requiredSize + 1];
    if (MultiByteToWideChar(65001 /*CP_UTF8*/, 0, Buffer, -1, super, requiredSize) == requiredSize)
    {
        return super;
    }

    return L'';
}

char* WinLayer::ConvertToNarrow(const wchar_t* Buffer)
{
    if (!Buffer)
    {
        MR_LOG(LogWindowsLayer, Error, "Attempted to convert an invalid buffer!");
        return '\0';
    }

    const int requiredSize = WideCharToMultiByte(65001 /*CP_UTF8*/, 0, Buffer, -1, 0, 0, 0, 0);
    if (requiredSize == 0)
    {
        MR_LOG(LogWindowsLayer, Error, "WideCharToMultiByte said: %s", GetError().Chr());
        return '\0';
    }

    char* super = new char[requiredSize + 1];
    if (WideCharToMultiByte(65001 /*CP_UTF8*/, 0, Buffer, -1, super, requiredSize, 0, 0) == requiredSize)
    {
        return super;
    }

    return '\0';
}

constexpr const int WinLayer::evaluateMessageBoxFlags(const int Code) const noexcept
{
    int flags = 0;

    if (Code & MESSAGEBOXTYPE_INFORMATION) flags |= MB_ICONINFORMATION;
    else if (Code & MESSAGEBOXTYPE_WARNING) flags |= MB_ICONWARNING;
    else if (Code & MESSAGEBOXTYPE_ERROR) flags |= MB_ICONERROR;

    if (Code & MESSAGEBOXBUTTONS_OK) flags |= MB_OK;
    else if ((Code & MESSAGEBOXBUTTONS_YES) && (Code & MESSAGEBOXBUTTONS_NO))
    {
        flags |= MB_YESNO;
    }
    else if (Code & MESSAGEBOXBUTTONS_YES)
    {
        MR_LOG(LogWindowsLayer, Verbose, "Button Yes supplied only, appending No");
        flags |= MB_YESNO;
    }
    else if (Code & MESSAGEBOXBUTTONS_NO)
    {
        MR_LOG(LogWindowsLayer, Verbose, "Button No supplied only, appending Yes");
        flags |= MB_YESNO;
    }
    else if (Code & MESSAGEBOXBUTTONS_CANCEL)
    {
        MR_LOG(LogWindowsLayer, Verbose, "Button Cancel supplied only, appending Ok");
        flags |= MB_OKCANCEL;
    }
    else
    {
        flags |= MB_OK;
    }

    return flags;
}

constexpr const MessageBoxDecision WinLayer::evaluateMessageBoxReturn(const int Code) const noexcept
{
    switch (Code)
    {
    case /* IDOK */ 1:
        return MESSAGEBOXDECISION_OK;
    case /* IDYES */ 6:
        return MESSAGEBOXDECISION_YES;
    case /* IDNO */ 7:
        return MESSAGEBOXDECISION_NO;    
    case /* IDRETRY */ 4:
        return MESSAGEBOXDECISION_RETRY;
    case /* IDCANCEL */ 2:
        return MESSAGEBOXDECISION_CANCEL;    
    case /* IDABORT */ 3:
        return MESSAGEBOXDECISION_STOP;

    default:
        return MESSAGEBOXDECISION_INVALID;
    }
}
