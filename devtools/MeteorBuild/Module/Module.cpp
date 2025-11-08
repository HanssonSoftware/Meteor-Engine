/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Module.h"
#include <FileManager.h>
#include <Platform/File.h>
#include <Core/Utils.h>

LOG_ADDCATEGORY(Parser);

void Module::Parse(String* modulePath)
{
	IFile* module = FileManager::CreateFileOperation(modulePath, FileAccessMode::OPENMODE_READ, FileShareMode::SHAREMODE_READ, OVERRIDERULE_OPEN_ONLY_IF_EXISTS);
	if (module != nullptr)
	{
		module->Read();

		const char* buffer = module->GetBuffer();

		if (Utils::GetWord(buffer, false) == "Module")
		{
			MR_LOG(LogParser, Verbose, "Opening %s as ModuleScript!", *module->GetName());

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
											MR_LOG(LogParser, Verbose, "Adding %s property to %s", *value, *flagWord);
										}

										if (Utils::GetCharacterType(buffer) == Comma)
											Utils::SkipCharacterType(buffer, Comma);
									}
								}
							}
							else if (Utils::GetCharacterType(buffer) != Colon)
							{
								MR_LOG(LogParser, Fatal, "Missing colon after word! %s", flagWord.Chr());
							}
							else
							{
								MR_LOG(LogParser, Fatal, "Unknown error!");
							}
						}

						Utils::SkipCharacterType(buffer, ClosedBrace);
					}
				}
			}
		}
		else if (Utils::GetWord(buffer, false) == "Project")
		{
			Utils::SkipWord(buffer);  // Skip "Project"

			MR_LOG(LogParser, Verbose, "Opening %s as ProjectScript!", *module->GetName());

			moduleName = Utils::GetWord(buffer, true);

			
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
										MR_LOG(LogParser, Verbose, "Adding %s property to %s", *value, *flagWord);
									}

									if (Utils::GetCharacterType(buffer) == Comma)
										Utils::SkipCharacterType(buffer, Comma);
								}
							}
						}
						else if (Utils::GetCharacterType(buffer) != Colon)
						{
							MR_LOG(LogParser, Fatal, "Missing colon after word! %s", flagWord.Chr());
						}
						else
						{
							MR_LOG(LogParser, Fatal, "Unknown error!");
						}
					}

					Utils::SkipCharacterType(buffer, ClosedBrace);
				}

				//buffer++;
			}
		}

		module->Close();
	}
}
