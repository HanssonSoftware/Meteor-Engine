/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "ModuleProcessor.h"
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

static void ExecutableWord(const String& arg)
{
	MR_LOG(LogScript, Log, "%s", arg.Chr());
}

ADD_SCRIPT_WORD(Executable, ExecutableWord);


void ModuleProcessor::ParseScript(const char* buffer, const ParsingType& type)
{
	switch (type)
	{
	case ParsingType::MainDescriptor:
		{
			// Parse main *.mrbuild script, which (should be) sits on the top of the passed directory.

			if (!Expected(buffer, "Solution"))
				return;

			SkipWhitspace(buffer);

			projectName = GetValue(buffer);
			if (projectName.IsEmpty())
				return;

			static uint32_t scopes = 0;
			while (!ExpectedIdentifier(buffer, TokenIdentifier::EndOfFile, true))
			{
				if (ExpectedIdentifier(buffer, TokenIdentifier::OpenBrace, true))
					scopes++;
				
				const String parsedWord = GetWord(buffer);
				if (!parsedWord)
				{
					if (ExpectedIdentifier(buffer, TokenIdentifier::OpenBrace, true))
					{
						scopes++;

						while (!ExpectedIdentifier(buffer, TokenIdentifier::CloseBrace, true))
							AdvanceACharacter(buffer);

						scopes--;
					}
				}


				if (ExpectedIdentifier(buffer, TokenIdentifier::Colon, true))
				{
					ScriptWordBase* word = ScriptWordBase::Find(parsedWord);
					if (!word)
					{
						MR_LOG(LogScript, Warn, "Invalid word: %s. Parsing would be skipped in that block.", parsedWord.Chr());
						
						if (ExpectedIdentifier(buffer, TokenIdentifier::OpenBrace, true))
						{
							scopes++;

							while (!ExpectedIdentifier(buffer, TokenIdentifier::CloseBrace, true))
								AdvanceACharacter(buffer);

							scopes--;
						}
					}

					const bool bCanStart = ExpectedIdentifier(buffer, TokenIdentifier::OpenBrace, true);
					if (bCanStart)
					{
						scopes++;
						SkipWhitspace(buffer);

						const String firstValue = GetValue(buffer);
						word->Invoke(firstValue);

						while (
							ExpectedIdentifier(buffer, TokenIdentifier::Comma, true) && 
							!ExpectedIdentifier(buffer, TokenIdentifier::CloseBrace, false))
						{
							SkipWhitspace(buffer);

							const String firstValue = GetValue(buffer);
							word->Invoke(firstValue);
						}
					}

					if (ExpectedIdentifier(buffer, TokenIdentifier::CloseBrace, true))
						scopes--;
				}
				else
				{
					MR_LOG(LogScript, Warn, "No colon passed after word: %s, Did you mean \"%s:\"?", parsedWord.Chr(), parsedWord.Chr());

					if (ExpectedIdentifier(buffer, TokenIdentifier::OpenBrace, true))
					{
						scopes++;

						while (!ExpectedIdentifier(buffer, TokenIdentifier::CloseBrace, true))
							AdvanceACharacter(buffer);

						scopes--;
					}
				}

				if (ExpectedIdentifier(buffer, TokenIdentifier::CloseBrace, false))
					AdvanceACharacter(buffer);
			}
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

bool ModuleProcessor::OpenScript(const String& modulePath)
{
	FileStatus stat;

	currentlyReadModule = FileManager::CreateFileOperation(modulePath, OPENMODE_READ, SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS, stat);
	if (!currentlyReadModule)
		return false;

	currentlyReadModule->Read();

	return true;
}


void ModuleProcessor::InputToContainer(ScriptWordBase& word)
{

	int32_t j = 4;
}

bool ModuleProcessor::Expected(const char*& in, const char* ptr)
{
	const char* begin = in;
	while (IsAlpha(*in)) in++;

	String temp(begin, (uint32_t)(in - begin));
	if (strcmp(temp.Chr(), ptr) > 0)
		return false;

	return true;
}

bool ModuleProcessor::ExpectedIdentifier(const char*& in, const int& identifier, bool bStep = false)
{
	SkipWhitspace(in);

	const bool bResult = GetIdentifier(in) == identifier;

	if (bResult && bStep)
		in++;

	return bResult;
}

int ModuleProcessor::GetIdentifier(const char*& in)
{
	if (IsWhitspace(in)) SkipWhitspace(in);

	if (*in == '{') return TokenIdentifier::OpenBrace;
	if (*in == '}') return TokenIdentifier::CloseBrace;
	if (*in == '\0') return TokenIdentifier::EndOfFile;
	if (*in == ':') return TokenIdentifier::Colon;
	if (*in == ',') return TokenIdentifier::Comma;
	
	return TokenIdentifier::Unknown;
}

String ModuleProcessor::TokenIndetifierToString(const TokenIdentifier& identifier) const
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

String ModuleProcessor::GetValue(const char*& in)
{
	in++;

	const char* begin = in;
	while (*in != '"') in++;

	String val(begin, (uint32_t)(in - begin));

	if (*in == '"')
		in++;

	return val;
}

String ModuleProcessor::GetWord(const char*& in)
{
	SkipWhitspace(in);

	const char* begin = in;
	while (IsAlpha(*in)) in++;

	String val(begin, (uint32_t)(in - begin));
	return val;
}