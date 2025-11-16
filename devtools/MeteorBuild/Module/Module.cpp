/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Module.h"
#include <FileManager.h>
#include <Platform/File.h>
#include <Core/Utils.h>

LOG_ADDCATEGORY(Parser);

static void AddVerbDetail(Module* moduleToWrite, const String& verb, const String& value)
{
	if (verb == "IncludePath" || verb == "IncludePaths") { moduleToWrite->includePaths.Add(value); return; }
	if (verb == "Dependencies") { moduleToWrite->requires.Add(value); return; }

	MR_LOG(LogParser, Error, "Unknown verb: %ls", *verb);
}

bool Module::Parse(String* modulePath)
{
	bool bFailed = false;

	IFile* module = FileManager::CreateFileOperation(modulePath, FileAccessMode::OPENMODE_READ, FileShareMode::SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS);
	if (module != nullptr)
	{
		module->Read();

		const char* buffer = module->GetBuffer();

		if (Utils::GetWord(buffer, false) == "Module")
		{
			MR_LOG(LogParser, Verbose, "Opening %ls as ModuleScript!", *module->GetName());

			Utils::SkipWord(buffer);  // Skip "Module"

			moduleName = Utils::GetWord(buffer, true);

			if (Utils::GetCharacterType(buffer) == Colon)
			{
				Utils::SkipCharacterType(buffer, Colon);
				dependsOn = Utils::GetWord(buffer, true);

				if (Utils::GetCharacterType(buffer) == OpenBrace)
				{
					Utils::SkipCharacterType(buffer, OpenBrace);

					while (*buffer != '\0')
					{
						while (Utils::GetCharacterType(buffer) != ClosedBrace
							&& Utils::GetCharacterType(buffer) != None)
						{
							const String flagWord = Utils::GetWord(buffer, true);
							if (flagWord && Utils::GetCharacterType(buffer) == Colon)
							{
								Utils::SkipCharacterType(buffer, Colon);

								if (Utils::GetCharacterType(buffer) == OpenBrace)
								{
									Utils::SkipCharacterType(buffer, OpenBrace);
									while (Utils::GetCharacterType(buffer) != ClosedBrace)
									{
										const String value = Utils::GetWord(buffer, true);
										if (value)
										{
											AddVerbDetail(this, flagWord, value);
											MR_LOG(LogParser, Verbose, "Adding %ls property to %ls", *value, *flagWord);
										}

										if (Utils::GetCharacterType(buffer) == Comma)
											Utils::SkipCharacterType(buffer, Comma);
									}
								}
							}
							else if (Utils::GetCharacterType(buffer) != Colon)
							{
								MR_LOG(LogParser, Fatal, "Missing colon after word %ls!", flagWord.Chr());
							}
							else
							{
								MR_LOG(LogParser, Fatal, "Unknown error!");
							}
						}

						Utils::SkipCharacterType(buffer, ClosedBrace);
					}

					if (*buffer == '\0')
					{
						if (SUCCEEDED(CoCreateGuid(&identification)))
						{
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
		return !bFailed;
	}

	return !bFailed;
}
