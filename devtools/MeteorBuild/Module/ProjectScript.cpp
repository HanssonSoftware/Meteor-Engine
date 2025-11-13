/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "ProjectScript.h"
#include <Core/Utils.h>
#include <FileManager.h>
#include <File.h>

LOG_ADDCATEGORY(ProjectScriptParser);

static void AddVerbDetail(const String& verb, const String& value)
{

}

bool ProjectScript::Finalize(String& output)
{
	output += "# This file is generated with MeteorBuild(R)\nMicrosoft Visual Studio Solution File, Format Version 12.00\nVisualStudioVersion = 17.11.35327.3\nMinimumVisualStudioVersion = 10.0.40219.1";
	
	return false;
}

bool ProjectScript::Parse(String* input)
{
	IFile* module = FileManager::CreateFileOperation(input, FileAccessMode::OPENMODE_READ, FileShareMode::SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS);
	if (module != nullptr)
	{
		module->Read();

		const char* buffer = module->GetBuffer();

		bool bHasBeenParsedOneWordAtLeast = false;
		if (Utils::GetWord(buffer, false) == "Project")
		{
			Utils::SkipWord(buffer);  // Skip "Project"

			MR_LOG(LogProjectScriptParser, Verbose, "Opening %s as ProjectScript!", *module->GetName());

			projectName = Utils::GetWord(buffer, true);

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
										if (!bHasBeenParsedOneWordAtLeast) bHasBeenParsedOneWordAtLeast = true;

										AddVerbDetail(flagWord, value);
										MR_LOG(LogProjectScriptParser, Verbose, "Adding %s property to %s", *value, *flagWord);
									}

									if (Utils::GetCharacterType(buffer) == Comma)
										Utils::SkipCharacterType(buffer, Comma);
								}
							}
						}
						else if (Utils::GetCharacterType(buffer) != Colon)
						{
							MR_LOG(LogProjectScriptParser, Fatal, "Missing colon after word! %s", flagWord.Chr());
						}
						else
						{
							MR_LOG(LogProjectScriptParser, Fatal, "Unknown error!");
						}
					}

					Utils::SkipCharacterType(buffer, ClosedBrace);
				}
			}
		}

		module->Close();
		return bHasBeenParsedOneWordAtLeast;
	}

	return false;
}
