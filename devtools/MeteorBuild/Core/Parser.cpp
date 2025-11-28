/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Parser.h"
#include <Module/Module.h>
#include <Module/Project.h>

#include <Platform/FileManager.h>

#include <rpc.h>
#include <Objbase.h>

LOG_ADDCATEGORY(Parser);

static void AddVerbDetail(Module* moduleToWrite, const String& verb, const String& value)
{
	if (verb == "IncludePath" || verb == "IncludePaths") { moduleToWrite->includePaths.Add(value); return; }
	if (verb == "Dependencies") { moduleToWrite->requires.Add(value); return; }

	MR_LOG(LogParser, Error, "Unknown verb: %ls", *verb);
}

Module* Parser::ParseModuleScript(String* moduleName)
{
	bool bFailed = false;

	IFile* module = FileManager::CreateFileOperation(moduleName, FileAccessMode::OPENMODE_READ, FileShareMode::SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS);
	if (module != nullptr)
	{
		module->Read();

		const char* buffer = module->GetBuffer();
		if (GetWord(buffer, false) == "Module")
		{
			MR_LOG(LogParser, Verbose, "Opening %ls as ModuleScript!", *module->GetName());
			Module newModule;

			SkipWord(buffer);  // Skip "Module"

			newModule.moduleName = GetWord(buffer, true);

			if (GetCharacterType(buffer) == Colon)
			{
				SkipCharacterType(buffer, Colon);
				newModule.dependsOn = GetWord(buffer, true);

				if (GetCharacterType(buffer) == OpenBrace)
				{
					SkipCharacterType(buffer, OpenBrace);

					while (*buffer != '\0')
					{
						while (GetCharacterType(buffer) != ClosedBrace
							&& GetCharacterType(buffer) != None)
						{
							const String flagWord = GetWord(buffer, true);
							if (flagWord && GetCharacterType(buffer) == Colon)
							{
								SkipCharacterType(buffer, Colon);

								if (GetCharacterType(buffer) == OpenBrace)
								{
									SkipCharacterType(buffer, OpenBrace);
									while (GetCharacterType(buffer) != ClosedBrace)
									{
										const String value = GetWord(buffer, true);
										if (value)
										{
											AddVerbDetail(&newModule, flagWord, value);
											MR_LOG(LogParser, Verbose, "Adding %ls property to %ls", *value, *flagWord);
										}

										if (GetCharacterType(buffer) == Comma)
											SkipCharacterType(buffer, Comma);
									}
								}
							}
							else if (GetCharacterType(buffer) != Colon)
							{
								MR_LOG(LogParser, Fatal, "Missing colon after word %ls!", flagWord.Chr());
							}
							else
							{
								MR_LOG(LogParser, Fatal, "Unknown error!");
							}
						}

						SkipCharacterType(buffer, ClosedBrace);
					}

					if (*buffer == '\0')
					{
						GUID id;
						if (SUCCEEDED(CoCreateGuid(&id)))
						{
							wchar_t buffer[64];
							if (!StringFromGUID2(id, buffer, 64))
							{
								module->Close();
								return false;
							}

							newModule.identification = buffer;
							MR_LOG(LogParser, Verbose, "Successfully generated GUID, for module %ls!", *moduleName);
						}
					}
				}
				else
				{
					bFailed = true;
				}
			}
			else
			{
				bFailed = true;
			}
		}
		else
		{
			bFailed = true;
		}

		module->Close();
		return nullptr;
	}

	return nullptr;
}

ECharacterType Parser::GetCharacterType(const char*& str)
{
	if (!str) return None;

	while (isspace(*str))
		str++;

	const char* constStr = str;
	if (isalpha(*constStr))
	{

		while (!isspace(*constStr))
			constStr++;

		return Word;
	}

	if (*constStr == '{')
	{
		return OpenBrace;
	}
	else if (*constStr == '}')
	{
		return ClosedBrace;
	}
	else if (*constStr == ',')
	{
		return Comma;
	}
	else if (*constStr == ';')
	{
		return SemiColon;
	}
	else if (*constStr == ':')
	{
		return Colon;
	}

	return None;
}

void Parser::SkipCharacterType(const char*& str, ECharacterType type)
{
	if (str)
	{
		switch (type)
		{
		case Word:
		{
			if (isalpha(*str))
			{
				while (!isspace(*str))
					str++;

				return;
			}
			break;
		}
		default:
			if (GetCharacterType(str) == type)
				str++;

			break;
		}
	}
}

String Parser::GetWord(const char*& in, bool bStep)
{
	while (isspace(*in))
		in++;

	const char* begin = in;
	const char* end = nullptr;
	uint32_t chars = 0;

	if (*begin == '"')
	{
		begin++; // "
		end = begin;

		while (*end && /*isalpha(*end)*/ *end != '"')
		{
			chars++;
			end++;
		}

		end++;
	}
	else if (*begin == '$')
	{
		end = begin;
		while (*end && !isspace(*end)) end++;
		chars = (uint32_t)(end - begin);
	}
	else
	{
		end = in;
		while (*end && !isspace(*end) && !ispunct(*end)) end++;

		chars = (uint32_t)(end - begin);
	}

	String returned(begin, chars);
	if (bStep) in = end;

	return returned;
}

void Parser::SkipWord(const char*& in)
{
	if (in)
	{
		while (*in && !isspace(*in))
			in++;
	}
}

bool Parser::IsSpace(const char* buffer)
{
	return *buffer == ' ' ? true : false;
}