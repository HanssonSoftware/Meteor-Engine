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
	String compileList, includeList;

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

	String includePath;
	for (auto& ic : includePaths)
	{
		includePath = String::Format(L"%ls;%ls", *includePath, *ic);
	}

	*output = String::Format(
	L"<!-- This file is generated with MeteorBuild(R) -->\n"
	L"<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\'http://schemas.microsoft.com/developer/msbuild/2003\'>\n"
	L"\t<ItemGroup Label = \"ProjectConfigurations\">\n"
	L"\t\t<ProjectConfiguration Include=\"Debug|x64\">\n"
	L"\t\t\t<Configuration>Debug</Configuration>\n"
	L"\t\t\t<Platform>x64</Platform>\n"
	L"\t\t</ProjectConfiguration>\n"
	L"\t\t<ProjectConfiguration Include=\"Shipping|x64\">\n"
	L"\t\t\t<Configuration>Shipping</Configuration>\n"
	L"\t\t\t<Platform>x64</Platform>\n"
	L"\t\t</ProjectConfiguration>\n"
	L"\t</ItemGroup>\n"
	L"\t<PropertyGroup Label=\"Globals\">\n"
	L"\t\t<VCProjectVersion>17.0</VCProjectVersion>\n"
	L"\t\t<Keyword>Win32Proj</Keyword>\n"
	L"\t\t<ProjectGuid>%ls</ProjectGuid>\n" // 1 project GUID
	L"\t\t<RootNamespace>%ls</RootNamespace>\n" // 2 namespace
	L"\t\t<WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>\n"
	L"\t\t<ProjectName>%ls</ProjectName>\n" // 3 project Name
	L"\t</PropertyGroup>\n"
	L"\t<Import Project = \"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n"
	L"\t<PropertyGroup Condition = \"'$(Configuration)|$(Platform)' == 'Debug|x64'\" Label = \"Configuration\">\n"
	L"\t\t<ConfigurationType>DynamicLibrary</ConfigurationType>\n"
	L"\t\t<PlatformToolset>v145</PlatformToolset>\n"
	L"\t\t<CharacterSet>Unicode</CharacterSet>\n"
	L"\t\t<UseDebugLibraries>true</UseDebugLibraries>\n"
	L"\t</PropertyGroup>\n"
	L"\t<PropertyGroup Condition = \"'$(Configuration)|$(Platform)' == 'Shipping|x64'\" Label = \"Configuration\">\n"
	L"\t\t<ConfigurationType>DynamicLibrary</ConfigurationType>\n"
	L"\t\t<PlatformToolset>v145</PlatformToolset>\n"
	L"\t\t<WholeProgramOptimization>true</WholeProgramOptimization>\n"
	L"\t\t<CharacterSet>Unicode</CharacterSet>\n"
	L"\t</PropertyGroup>\n"

	L"\t<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\"/>\n"
	L"\t<ImportGroup Label=\"ExtensionSettings\">\n"
	L"\t</ImportGroup>\n"
	L"\t<ImportGroup Label=\"Shared\">\n"
	L"\t</ImportGroup>\n"
	L"\t<ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\n"
    L"\t\t<Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\"/>\n"
	L"\t</ImportGroup>\n"
	L"\t<ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Shipping|x64'\">\n"
    L"\t\t<Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\"/>\n"
	L"\t</ImportGroup>\n"
	L"\t<PropertyGroup Label=\"UserMacros\" />\n"
	
	L"\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)' == 'Debug|x64'\">\n"
	L"\t\t<OutDir>$(SolutionDir)Product\\</OutDir>\n"
	L"\t\t<IntDir>$(SolutionDir)Intermediate\\$(ProjectName)\\</IntDir>\n"
	L"\t\t<TargetName>MeteorEngine-%ls</TargetName>\n" // 4 module name
	L"\t\t<IncludePath>$(IncludePath)</IncludePath>\n"
	L"\t\t<LibraryPath>$(LibraryPath)</LibraryPath>\n"
	L"\t</PropertyGroup>\n"
	L"\t<PropertyGroup Condition = \"'$(Configuration)|$(Platform)'=='Shipping|x64'\">\n"
	L"\t\t<OutDir>$(SolutionDir)Product\\</OutDir>\n"
	L"\t\t<IntDir>$(SolutionDir)Intermediate\\$(ProjectName)\\</IntDir>\n"
	L"\t\t<TargetName>MeteorEngine-%ls</TargetName>\n" // 4 module name
	L"\t\t<IncludePath>$(IncludePath)</IncludePath>\n"
	L"\t\t<LibraryPath>$(LibraryPath)</LibraryPath>\n"
	//L"\t<TargetExt>.lib</TargetExt>
	L"\t</PropertyGroup>\n"

	L"\t<ItemDefinitionGroup Condition=\"\'$(Configuration)|$(Platform)\' == \'Debug|x64\'\">\n"
	L"\t\t<ClCompile>\n"
	L"\t\t\t<PreprocessorDefinitions>MR_DEBUG;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
	L"\t\t\t<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>\n"
	L"\t\t\t<LanguageStandard>stdcpp17</LanguageStandard>\n"
	L"\t\t\t<Optimization>Disabled</Optimization>\n"
	L"\t\t\t<BasicRuntimeChecks>EnableFastChecks</BasicRuntimeChecks>\n"
	L"\t\t\t<AdditionalIncludeDirectories>%ls</AdditionalIncludeDirectories>\n" // 5 include paths
	L"\t\t</ClCompile>\n"
	L"\t</ItemDefinitionGroup>\n"
	L"\t<ItemDefinitionGroup Condition=\"\'$(Configuration)|$(Platform)\' == \'Shipping|x64\'\">\n"
	L"\t\t<ClCompile>\n"
	L"\t\t\t<LanguageStandard>stdcpp17</LanguageStandard>\n"
	L"\t\t\t<PreprocessorDefinitions>%%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
	L"\t\t\t<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>\n"
	L"\t\t\t<Optimization>MaxSpeed</Optimization>\n"
	L"\t\t\t<AdditionalIncludeDirectories>%ls</AdditionalIncludeDirectories>\n" // 5 include paths
	L"\t\t</ClCompile>\n"
	L"\t</ItemDefinitionGroup>\n"

	L"\t<ItemGroup>\n"
	L"%ls"
	L"\t</ItemGroup>\n"
	L"\t<ItemGroup>\n"
	L"%ls"
	L"\t</ItemGroup>\n"
	L"\t<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\n"
	L"\t<ImportGroup Label=\"ExtensionTargets\" >\n"
	L"\t</ImportGroup>\n"
	L"</Project>",
	*identification,
	*moduleName,
	*moduleName,
	*moduleName,
	*moduleName,
	*includePath,
	*includePath,
	*compileList,
	*includeList
	);

	return true;
}
