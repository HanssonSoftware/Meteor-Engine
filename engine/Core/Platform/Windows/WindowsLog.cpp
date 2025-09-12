/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "WindowsLog.h"
#include <Windows/Windows.h>
#include <Logging/LogMacros.h>
#include <Application.h>
#include <Layers/SystemLayer.h>
#include <Version.h>
#include "WindowsFile.h"

#include <DbgHelp.h>
#include <assertbox.h>
#pragma warning(disable : 6001) // Using uninitialized memory 'X'

LOG_ADDCATEGORY(AssertionSystem);

static HANDLE hConsole;

WindowsLog::WindowsLog()
	: ILogger()
{
	
}

WindowsLog::~WindowsLog() noexcept
{

}

void WindowsLog::Initialize()
{
#ifdef MR_DEBUG
	if constexpr (bIsRunningDebugMode)
	{
		FreeConsole();

		if (!AllocConsole())
		{
			if (GetLastError() == ERROR_ACCESS_DENIED)
			{
				if (!AttachConsole(ATTACH_PARENT_PROCESS))
				{
					Application::RequestExit(-1);
				}
			}
			else
			{
				Application::RequestExit(-1);
			}
		}
		else
		{
			if (!SetStdHandle(STD_INPUT_HANDLE, INVALID_HANDLE_VALUE))
			{
				Application::RequestExit(-1);
			}

			hConsole = CreateFileW(L"CONOUT$", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
			if (hConsole != INVALID_HANDLE_VALUE)
			{
				SetConsoleOutputCP(CP_UTF8);

				SetConsoleCP(CP_UTF8);

				CONSOLE_CURSOR_INFO cf;
				GetConsoleCursorInfo(hConsole, &cf);
				cf.bVisible = false;

				SetConsoleCursorInfo(hConsole, &cf);

			
				if (SystemLayer* systemLayer = Layer::GetSystemLayer())
				{
					if (!SetStdHandle(STD_OUTPUT_HANDLE, hConsole))
						Application::RequestExit(-1);

					wchar_t* buffer = systemLayer->ConvertToWide(String::Format("%s developer console (b%d)", Application::Get()->appName.Chr(), BUILD_NUMBER).Chr());

					if (!SetConsoleTitleW(buffer))
					{
						delete[] buffer;
						Application::RequestExit(-1);
					}

					delete[] buffer; // Using uninitialized memory 'X'
				}
			}
		}
	}
#endif // MR_DEBUG

	ILogger::Initialize();
}

void WindowsLog::Shutdown()
{
#ifdef MR_DEBUG
	if constexpr (bIsRunningDebugMode)
	{
		CloseHandle(hConsole);

		if (!FreeConsole())
		{
			Application::RequestExit(-1);
		}
	}
#endif // MR_DEBUG

	ILogger::Shutdown();
}


void WindowsLog::SendToOutputBuffer(const String& Buffer)
{
#ifdef MR_DEBUG
	if constexpr (bIsRunningDebugMode)
	{
		if (SystemLayer* systemLayer = Layer::GetSystemLayer())
		{
			wchar_t* message = systemLayer->ConvertToWide(Buffer.Chr());

			const LogDescriptor* actualDescriptor = ILogger::Get()->GetActualEntry();
			if (!actualDescriptor)
			{
				delete[] message;
				return;
			}

			switch (actualDescriptor->severity)
			{
			case Log:
				SetConsoleTextAttribute(hConsole, 0x7);
				break;
			case Verbose:
				SetConsoleTextAttribute(hConsole, 0x9);
				break;
			case Error:
				SetConsoleTextAttribute(hConsole, 0xC);
				break;
			case Warn:
				SetConsoleTextAttribute(hConsole, 0x6);
				break;
			case Fatal:
				SetConsoleTextAttribute(hConsole, 0x4);
				break;
			}

			DWORD written = 0;
			if (!WriteConsoleW(hConsole, message, (DWORD)wcslen(message), &written, 0))
				return ILogger::SendToOutputBuffer(Buffer);

			if (IsDebuggerAttached())
				OutputDebugStringW(message);

			delete[] message;
		}
	}
#endif // MR_DEBUG

	ILogger::SendToOutputBuffer(Buffer);
}

void WindowsLog::HandleFatal()
{
	if (SystemLayer* systemLayer = Layer::GetSystemLayer())
	{
		const LogDescriptor* actualDescriptor = ILogger::Get()->GetActualEntry();
		if (!actualDescriptor)
		{
			MessageBoxW(nullptr, L"Unknown error!", L"Engine Error!", MB_OK);

			TerminateProcess(GetCurrentProcess(), -1);
			return;
		}

		const wchar_t* convertedFatalText = systemLayer->ConvertToWide(actualDescriptor->message.Chr());

		MessageBoxW(nullptr, convertedFatalText, L"Engine Error!", MB_OK);
		delete[] convertedFatalText;
	}
	else
	{
		MessageBoxW(nullptr, L"Unknown error!", L"Engine Error!", MB_OK);
	}

	TerminateProcess(GetCurrentProcess(), -1);
}

bool WindowsLog::IsDebuggerAttached()
{
	return IsDebuggerPresent() ? true : false;
}


static INT_PTR WindowsLoggingDialogProcedure(HWND wnd, UINT msg, WPARAM ai1, LPARAM ai2)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		if (const LogAssertion* pkg = reinterpret_cast<const LogAssertion*>(ai2))
		{
			SystemLayer* systemLayer = Layer::GetSystemLayer();
			if (systemLayer)
			{
				wchar_t* fileName = systemLayer->ConvertToWide(pkg->assertLocationInFile);
				SetDlgItemTextW(wnd, IDC_ASSERTIONFILESTATEMENT, fileName);

				wchar_t* statement = systemLayer->ConvertToWide(pkg->assertStatement);
				SetDlgItemTextW(wnd, IDC_ASSERTIONSTATEMENT, statement);

				wchar_t* line = systemLayer->ConvertToWide(String(pkg->assertLineInFile));
				SetDlgItemTextW(wnd, IDC_ASSERTIONLINESTATEMENT, line);
				
				delete[] statement, fileName, line;

				if (!Logger::IsDebuggerAttached())
					EnableWindow(GetDlgItem(wnd, IDBREAKONDEBUGGER), 0);

				if (!pkg->assertMessage.IsEmpty())
				{
					wchar_t* message = systemLayer->ConvertToWide(pkg->assertMessage.Chr());
					SetDlgItemTextW(wnd, IDC_ASSERTIONLINEMESSAGE, message);

					delete[] message;
				}
				else
				{
					ShowWindow(GetDlgItem(wnd, IDC_ASSERTIONLINEMESSAGE), SW_HIDE);
					ShowWindow(GetDlgItem(wnd, IDC_ASSERTIONMESSAGE), SW_HIDE);
				}
			}
			
		}
	break;
	case WM_COMMAND:
		switch (LOWORD(ai1))
		{
		case IDBREAKONDEBUGGER:
			EndDialog(wnd, 1);
			return TRUE;
		case IDIGNORETHIS:
			EndDialog(wnd, 2);
			return TRUE;
		case IDEXIT:
			EndDialog(wnd, -2);
			return TRUE;



		default:
			break;
		}

	break;

	case WM_CLOSE:
		EndDialog(wnd, -2);
		return TRUE;

	default:
		return DefWindowProcW(wnd, msg, ai1, ai2);
	}

	return DefWindowProcW(wnd, msg, ai1, ai2);
}

int32_t WindowsLog::TransmitAssertion(LogAssertion& Info)
{
	if (Info.bIgnoreThis || Info.bIgnoreFor > 0)
		return -1;

	auto result = DialogBoxParamW(
		GetModuleHandleW(0), 
		MAKEINTRESOURCEW(IDD_ASSERTIONDIALOG), 
		0, 
		WindowsLoggingDialogProcedure, 
		(LPARAM)&Info
	);

	switch (result)
	{
	case -2:
		ExitProcess(-1);
		break;
	case 2:		
		Info.bIgnoreThis = true;
		MR_LOG(LogAssertionSystem, Warn, "Assert is supressed! %s:%d", Info.assertLocationInFile, Info.assertLineInFile);
		break;

	default:
		break;
	}

	if (SystemLayer* systemLayer = Layer::GetSystemLayer())
	{
		const String j = systemLayer->GetError();
	}

	return (int32_t)result;
}