/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "ScriptParser.h"
#include <Logging/LogMacros.h>
#include "Module.h"
#include <FileManager.h>
#include <Commandlet.h>
#include <Layers/SystemLayer.h>

#include <Windows/Windows.h>
#include <Windows/WindowsPaths.h>
#include <PathCch.h>
#include <shlwapi.h>
#include <strsafe.h>

#pragma comment(lib, "Shlwapi.lib")

LOG_ADDCATEGORY(Script);

ADD_SCRIPT_WORD(Executable);

void ScriptParser::ParseScript(const ParsingType& type)
{
	switch (type)
	{
	case ParsingType::MainDescriptor:
		{
			// Parse main *.mrbuild script, which (should be) sits on the top of the passed directory.

			const char* t = currentlyReadModule->GetBuffer();
			if (t == nullptr)
				return;

			if (!Expected(t, "Solution"))
				return;

			SkipWhitspace(t);

			projectName = GetValue(t);
			if (projectName.IsEmpty())
				return;

			if (!ExpectedIdentifier(t, TokenIdentifier::OpenBrace))
				return;

			SkipWhitspace(t);
			AdvanceACharacter(t);
			SkipWhitspace(t);
			
			while (!ExpectedIdentifier(t, TokenIdentifier::EndOfFile))
			{
				const String fasz = GetWord(t);
				if (!fasz.IsEmpty() && ExpectedIdentifier(t, TokenIdentifier::Colon))
				{
					AdvanceACharacter(t);
					SkipWhitspace(t);

					if (!ExpectedIdentifier(t, TokenIdentifier::OpenBrace))
					{
						MR_LOG(LogScript, Fatal, "Identifier at %c is not %s", *t, *TokenIndetifierToString(TokenIdentifier::OpenBrace));
					}

					AdvanceACharacter(t);

					bool bShouldClose = false;
					while (!ExpectedIdentifier(t, TokenIdentifier::Comma))
					{
						SkipWhitspace(t);

						executable.Add(GetValue(t));
						
						ScriptWord_Executable  fasz;
						InputToContainer(fasz);
						if (!ExpectedIdentifier(t, TokenIdentifier::Comma)) 
						{ 
							AdvanceACharacter(t);
							break; 
						};
					}
				}

				if (ExpectedIdentifier(t, TokenIdentifier::CloseBrace)) AdvanceACharacter(t);
			}

			int j = 523;
		}

		break;
	case ParsingType::Module:
		{
			


		}

		break;
	default:
		break;
	}
}

bool ScriptParser::OpenScript(const String& modulePath)
{
	FileStatus stat;

	currentlyReadModule = FileManager::CreateFileOperation(modulePath, OPENMODE_READ, SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS, stat);
	if (!currentlyReadModule)
		return false;

	currentlyReadModule->Read();

	return true;
}

bool ScriptParser::FindMainScript(String& path)
{
	wchar_t* fullPath = nullptr;

	bool bIsPathWasRelative = false;
	if (FileManager::IsPathRelative(path))
	{
		wchar_t* exeDir = Layer::GetSystemLayer()->ConvertToWide(Paths::GetExecutableDirctory());
		wchar_t* convertedPath = Layer::GetSystemLayer()->ConvertToWide(path);

		PathCchRemoveFileSpec(exeDir, wcslen(exeDir));
		if (FAILED(PathAllocCombine(exeDir, convertedPath, PATHCCH_ALLOW_LONG_PATHS, &fullPath)))
		{
			MR_LOG(LogScript, Fatal, "PathAllocCombine returned: %s", *Layer::GetSystemLayer()->GetError())
		}

		bIsPathWasRelative = true;
		delete[] exeDir, convertedPath;
	}
	else
	{
		fullPath = Layer::GetSystemLayer()->ConvertToWide(path);
	}

	StringCchCatW(fullPath, wcslen(fullPath) + wcslen(L"\\*") + 1, L"\\*");

	WIN32_FIND_DATAW data;
	HANDLE firstFound = FindFirstFileW(fullPath, &data);

	if (firstFound != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (data.cFileName[0] == '.')
				continue;

			if (FileManager::IsEndingWith(data.cFileName, "mrbuild"))
			{
				fullPath[wcslen(fullPath) - 2] = L'\0';

				path = String::Format("%ls\\%ls", fullPath, data.cFileName);
				break;
			}


		} while (FindNextFileW(firstFound, &data));

		if (bIsPathWasRelative)
			LocalFree(fullPath);
		else
			delete[] fullPath;

		LocalFree(firstFound);
		return true;
	}
	else
	{
		MR_LOG(LogScript, Fatal, "FindFirstFileW encountered an invalid handle! %s", *Layer::GetSystemLayer()->GetError());
	}

	if (bIsPathWasRelative)
		LocalFree(fullPath);
	else
		delete[] fullPath;

	return false;
}

void ScriptParser::InputToContainer(ScriptWordBase& word)
{
	String a = word.GetWord();
	word.container;

	int j = 4;
}

bool ScriptParser::Expected(const char*& in, const char* ptr)
{
	const char* begin = in;
	while (IsAlpha(*in)) in++;

	String temp(begin, (uint32)(in - begin));
	if (strcmp(temp.Chr(), ptr) > 0)
		return false;

	return true;
}

bool ScriptParser::ExpectedIdentifier(const char*& in, const TokenIdentifier& identifier)
{
	if (IsWhitspace(in)) SkipWhitspace(in);

	return (GetIdentifier(in) == identifier) ? true : false;
}

TokenIdentifier ScriptParser::GetIdentifier(const char*& in)
{
	if (IsWhitspace(in)) SkipWhitspace(in);

	if (*in == '{') return TokenIdentifier::OpenBrace;
	if (*in == '}') return TokenIdentifier::CloseBrace;
	if (*in == '\0') return TokenIdentifier::EndOfFile;
	if (*in == ':') return TokenIdentifier::Colon;
	if (*in == ',') return TokenIdentifier::Comma;
	
	return TokenIdentifier::Unknown;
}

String ScriptParser::TokenIndetifierToString(const TokenIdentifier& identifier) const
{
	switch (identifier)
	{
	case TokenIdentifier::Identifier:	// Project, IncludePath, Modules
		return "any of known words, like: Project, IncludePaths, Modules";

	case TokenIdentifier::StringLiteral:	 // "Renderer", "Meteor", "CoreKit"
		return "user inputted string";

	case TokenIdentifier::Colon:		// :
		return ":";

	case TokenIdentifier::Comma:		// ,
		return ",";

	case TokenIdentifier::OpenBrace:	// {
		return "{";

	case TokenIdentifier::CloseBrace:	// }
		return "}";

	case TokenIdentifier::EndOfFile:	// \0
		return "null terminator";
	
	case TokenIdentifier::Unknown:
		return "";
	}

	return "";
}

String ScriptParser::GetValue(const char*& in)
{
	in++;

	const char* begin = in;
	while (*in != '"') in++;

	String val(begin, (uint32)(in - begin));

	if (*in == '"')
		in++;

	return val;
}

String ScriptParser::GetWord(const char*& in)
{
	const char* begin = in;
	while (IsAlpha(*in)) in++;

	String val(begin, (uint32)(in - begin));
	return val;
}