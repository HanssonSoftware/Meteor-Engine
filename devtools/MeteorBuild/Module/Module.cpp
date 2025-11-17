/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Module.h"
#include <FileManager.h>
#include <Platform/File.h>
#include <Core/Utils.h>

#include <rpc.h>
#include <Objbase.h>

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
		this->modulePath = *modulePath;

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
						GUID id;
						if (SUCCEEDED(CoCreateGuid(&id)))
						{
							wchar_t buffer[64];
							if (!StringFromGUID2(id, buffer, 64))
							{
								module->Close();
								return false;
							}

							identification = buffer;
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

bool Module::ConstructProjectFile(String* output)
{
	String compileList;
	String includeList;

	for (auto& cl : files)
	{
		if (FileManager::IsEndingWith(cl, "h"))
		{
			String actual = String::Format(L"\t\t<ClInclude Include=\"%ls\" />\n", *cl);

			includeList = String::Format(L"%ls%ls", *includeList, *actual);
		}
		else if (FileManager::IsEndingWith(cl, "cpp"))
		{
			String actual = String::Format(L"\t\t<ClCompile Include=\"%ls\" />\n", *cl);
			compileList = String::Format(L"%ls%ls", *compileList, *actual);
		}
	}

	*output = String::Format(
	L"<!-- This file is generated with MeteorBuild(R) -->\n"
	L"<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\'http://schemas.microsoft.com/developer/msbuild/2003\'>\n"
	L"\t<ItemGroup Label = \"ProjectConfigurations\" />\n"
	L"\t<PropertyGroup Label=\"Globals\" />\n"
	L"\t<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.default.props\" />\n"
	L"\t<PropertyGroup Label=\"Configuration\" />\n"
	L"\t<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n"
	L"\t<ImportGroup Label=\"ExtensionSettings\" />\n"
	L"\t<ImportGroup Label=\"PropertySheets\" />\n"
	L"\t<PropertyGroup Label=\"UserMacros\" />\n"
	L"\t<PropertyGroup />\n"
	L"\t<ItemDefinitionGroup />\n"
	L"\t<ItemDefinitionGroup Condition=\"\'$(Configuration)|$(Platform)\' == \'Shipping|x64\'\">\n"
	L"\t<ClCompile>\n"
	L"\t\t<PreprocessorDefinitions>%%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
	L"\t</ClCompile>\n"
	L"\t</ItemDefinitionGroup>\n"
	L"\t<ItemDefinitionGroup Condition=\"\'$(Configuration)|$(Platform)\' == \'Debug|x64\'\">\n"
	L"\t<ClCompile>\n"
	L"\t\t<PreprocessorDefinitions>MR_DEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
	L"\t</ClCompile>\n"
	L"\t</ItemDefinitionGroup>\n"
	L"\t<ItemGroup>\n"
	L"%ls"
	L"\t</ItemGroup>\n"
	L"\t<ItemGroup>\n"
	L"%ls"
	L"\t</ItemGroup>\n"
	L"\t<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\n"
	L"\t<ImportGroup Label=\"ExtensionTargets\" />\n"
	L"</Project>", 
	*compileList,
	*includeList
	);

	return true;
}
