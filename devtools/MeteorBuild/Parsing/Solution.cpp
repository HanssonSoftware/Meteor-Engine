/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#define _CRT_SECURE_NO_WARNINGS
#include "Solution.h"

#pragma warning(disable : 4996)
#pragma warning(disable : 6031)
#pragma warning(disable : 6001)
#pragma warning(disable : 6387)

#include <Log/LogMacros.h>

LOG_ADDCATEGORY(Project);


/*

Microsoft Visual Studio Solution File, Format Version 12.00		|
# Visual Studio Version 17										|		PRE GENERATED SECTION
VisualStudioVersion = 17.11.35327.3								|
MinimumVisualStudioVersion = 10.0.40219.1						|


			|		  THIS IS CONSTEXPR			 |		| Display Name |	| Project Name |		|			   It's guid			   |
Project("	{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}	")	 =	"Meteor"	,	"meteor.vcxproj"	, "{709B7964-F338-40B8-B8E2-A2898B438A1C}"		|
EndProject																																		|		A PROJECT DEFINITION


Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Editor", "editor\Editor.vcxproj", "{13CDFF27-4E0F-4C92-92EC-F6D925CD8D09}"								| REPEAT 1.
EndProject																																					| REPEAT 1.
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "MeteorBuild", "devtools\MeteorBuild\MeteorBuild.vcxproj", "{E75C5354-0747-4518-9E9B-B4FDFF214314}"		| REPEAT 2.
EndProject																																					| REPEAT 2.

Global

	GlobalSection(SolutionConfigurationPlatforms) = preSolution				  |
		Debug|x64 = Debug|x64												  |
		Shipping|x64 = Shipping|x64											  |		MAIN CONFIG SECTION FOR ALL PROJECTS
	EndGlobalSection														  |

	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		{709B7964-F338-40B8-B8E2-A2898B438A1C}.Debug|x64.ActiveCfg = Debug|x64
		{709B7964-F338-40B8-B8E2-A2898B438A1C}.Debug|x64.Build.0 = Debug|x64
		{709B7964-F338-40B8-B8E2-A2898B438A1C}.Shipping|x64.ActiveCfg = Shipping|x64
		{709B7964-F338-40B8-B8E2-A2898B438A1C}.Shipping|x64.Build.0 = Shipping|x64
		{13CDFF27-4E0F-4C92-92EC-F6D925CD8D09}.Debug|x64.ActiveCfg = Debug|x64
		{13CDFF27-4E0F-4C92-92EC-F6D925CD8D09}.Debug|x64.Build.0 = Debug|x64
		{13CDFF27-4E0F-4C92-92EC-F6D925CD8D09}.Shipping|x64.ActiveCfg = Shipping|x64
		{13CDFF27-4E0F-4C92-92EC-F6D925CD8D09}.Shipping|x64.Build.0 = Shipping|x64
		{E75C5354-0747-4518-9E9B-B4FDFF214314}.Debug|x64.ActiveCfg = Debug|x64
		{E75C5354-0747-4518-9E9B-B4FDFF214314}.Debug|x64.Build.0 = Debug|x64
		{E75C5354-0747-4518-9E9B-B4FDFF214314}.Shipping|x64.ActiveCfg = Debug|x64
		{E75C5354-0747-4518-9E9B-B4FDFF214314}.Shipping|x64.Build.0 = Debug|x64
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	GlobalSection(ExtensibilityGlobals) = postSolution
		SolutionGuid = {7442A397-EEB5-400C-B726-B1A2CAA77F66}
	EndGlobalSection
EndGlobal
*/

static constexpr const wchar_t* solutionGUIDs[]
{
	L"ASP.NET 5|{8BB2217D-0F2D-49D1-97BC-3654ED321F3B}",
	L"ASP.NET MVC 1|{603C0E0B-DB56-11DC-BE95-000D561079B0}",
	L"ASP.NET MVC 2|{F85E285D-A4E0-4152-9332-AB1D724D3325}",
	L"ASP.NET MVC 3|{E53F8FEA-EAE0-44A6-8774-FFD645390401}",
	L"ASP.NET MVC 4|{E3E379DF-F4C6-4180-9B81-6769533ABE47}",
	L"ASP.NET MVC 5|{349C5851-65DF-11DA-9384-00065B846F21}",
	L"C#|{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}",
	L"C# DLL|{9A19103F-16F7-4668-BE54-9A1E7A4F7556}",
	L"C++|{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}",
	L"Database|{A9ACE9BB-CECE-4E62-9AA4-C7E7C5BD2124}",
	L"Database (other project types)|{4F174C21-8C12-11D0-8340-0000F80270F8}",
	L"Deployment Cab|{3EA9E505-35AC-4774-B492-AD1749C4943A}",
	L"Deployment Merge Module|{06A35CCD-C46D-44D5-987B-CF40FF872267}",
	L"Deployment Setup|{978C614F-708E-4E1A-B201-565925725DBA}",
	L"Deployment Smart Device Cab|{AB322303-2255-48EF-A496-5904EB18DA55}",
	L"Distributed System|{F135691A-BF7E-435D-8960-F99683D2D49C}",
	L"Dynamics 2012 AX C# in AOT|{BF6F8E12-879D-49E7-ADF0-5503146B24B8}",
	L"F#|{F2A71F9B-5D33-465A-A702-920D77279786}",
	L"J#|{E6FDF86B-F3D1-11D4-8576-0002A516ECE8}",
	L"Legacy (2003) Smart Device (C#)|{20D4826A-C6FA-45DB-90F4-C717570B9F32}",
	L"Legacy (2003) Smart Device (VB.NET)|{CB4CE8C6-1BDB-4DC7-A4D3-65A1999772F8}",
	L"Micro Framework|{b69e3092-b931-443c-abe7-7e7b65f2a37f}",
	L"Model-View-Controller v2 (MVC 2)|{F85E285D-A4E0-4152-9332-AB1D724D3325}",
	L"Model-View-Controller v3 (MVC 3)|{E53F8FEA-EAE0-44A6-8774-FFD645390401}",
	L"Model-View-Controller v4 (MVC 4)|{E3E379DF-F4C6-4180-9B81-6769533ABE47}",
	L"Model-View-Controller v5 (MVC 5)|{349C5851-65DF-11DA-9384-00065B846F21}",
	L"Mono for Android|{EFBA0AD7-5A72-4C68-AF49-83D382785DCF}",
	L"MonoTouch|{6BC8ED88-2882-458C-8E55-DFD12B67127B}",
	L"MonoTouch Binding|{F5B4F3BC-B597-4E2B-B552-EF5D8A32436F}",
	L"Portable Class Library|{786C830F-07A1-408B-BD7F-6EE04809D6DB}",
	L"Project Folders|{66A26720-8FB5-11D2-AA7E-00C04F688DDE}",
	L"SharePoint (C#)|{593B0543-81F6-4436-BA1E-4747859CAAE2}",
	L"SharePoint (VB.NET)|{EC05E597-79D4-47f3-ADA0-324C4F7C7484}",
	L"SharePoint Workflow|{F8810EC1-6754-47FC-A15F-DFABD2E3FA90}",
	L"Silverlight|{A1591282-1198-4647-A2B1-27E5FF5F6F3B}",
	L"Smart Device (C#)|{4D628B5B-2FBC-4AA6-8C16-197242AEB884}",
	L"Smart Device (VB.NET)|{68B1623D-7FB9-47D8-8664-7ECEA3297D4F}",
	L"Solution Folder|{2150E333-8FDC-42A3-9474-1A3956D46DE8}",
	L"Test|{3AC096D0-A1C2-E12C-1390-A8335801FDAB}",
	L"Universal Windows Class Library|{A5A43C5B-DE2A-4C0C-9213-0A381AF9435A}",
	L"VB.NET|{F184B08F-C81C-45F6-A57F-5ABD9991F28F}",
	L"Visual Database Tools|{C252FEB5-A946-4202-B1D4-9916A0590387}",
	L"Visual Studio 2015 Installer Project Extension|{54435603-DBB4-11D2-8724-00A0C9A8B90C}",
	L"Visual Studio Tools for Applications (VSTA)|{A860303F-1F3F-4691-B57E-529FC101A107}",
	L"Visual Studio Tools for Office (VSTO)|{BAA0C2D2-18E2-41B9-852F-F413020CAA33}",
	L"Web Application|{349C5851-65DF-11DA-9384-00065B846F21}",
	L"Web Site|{E24C65DC-7377-472B-9ABA-BC803B73C61A}",
	L"Windows (C#)|{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}",
	L"Windows (VB.NET)|{F184B08F-C81C-45F6-A57F-5ABD9991F28F}",
	L"Windows (Visual C++)|{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}",
	L"Windows Communication Foundation (WCF)|{3D9AD99F-2412-4246-B90B-4EAA41C64699}",
	//L"Windows Phone 8/8.1 Blank/Hub/Webview App|{76F1466A-8B6D-4E39-A767-685A06062A39}"
	//L"Windows Phone 8/8.1 App (C#)|{C089C8C0-30E0-4E22-80C0-CE093F111A43}"
	//L"Windows Phone 8/8.1 App (VB.NET)|{DB03555F-0C8B-43BE-9FF9-57896B3C5E56}"
	L"Windows Presentation Foundation (WPF)|{60DC8134-EBA5-43B8-BCC9-BB4BC16C2548}",
	L"Windows Store (Metro) Apps & Components|{BC8A1FFA-BEE3-4634-8014-F334798102B3}",
	L"Workflow (C#)|{14822709-B5A1-4724-98CA-57A101D1B079}",
	L"Workflow (VB.NET)|{D59BE175-2ED0-4C54-BE3D-CDAA9F3214C8}",
	L"Workflow Foundation|{32F31D43-81CC-4C15-9DE6-3FC5453562B6}",
	L"Xamarin.Android|{EFBA0AD7-5A72-4C68-AF49-83D382785DCF}",
	L"Xamarin.iOS|{6BC8ED88-2882-458C-8E55-DFD12B67127B}",
	L"XNA (Windows)|{6D335F3A-9D43-41b4-9D22-F6F17C4BE596}",
	L"XNA (XBox)|{2DF5C3F4-5A5F-47a9-8E94-23B4456F55E2}",
	L"XNA (Zune)|{D399B71A-8929-442a-A9AC-8BEC78BB2433}",
};

Solution::Solution(const String Name)
	: mainProjectName(Name)
{

}

Project::Project(String displayName, String projectNameWithExtension, const String projectTypeGUID, const String projectUniqueGUID)
	: projectName()
	, projectNamePath(projectNameWithExtension)
{
	if (displayName.isEmpty())
	{
		String projName = projectNameWithExtension.Chr();
		MR_LOG(LogProject, Warn, TEXT("No Display Name Provided! Appending Project Name Instead. (%s)"), wcstok(projName.Data(), L"."));
		projectName = projName;
	}
	else
	{
		projectName = displayName;
	}

	bool bFound = false;
	static const wchar_t* projGUID = projectTypeGUID.isEmpty() ? /* Visual C++ project. */ L"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}" : projectTypeGUID.Chr();
	for (const wchar_t* temp : solutionGUIDs)
	{
		//if (!temp) continue; // This will never be NULL!
		
		wchar_t nonConstBlock[256];
		wcscpy(nonConstBlock, temp);

		wchar_t* token;
		wcstok(nonConstBlock, L"|", &token);
		if (wcscmp(projGUID, token) == 0)
		{
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		MR_LOG(LogProject, Error, TEXT("Broken or Undefined GUID was Found!"));
	}
}

String Project::formatToOutput() const
{
	static String plot;
	if (!plot.isEmpty())
	{
		return plot;
	}

	//wchar_t guidBuffer[40];
	//swprintf(guidBuffer, 
	//	L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}", 
	//	selfProjectGuid.Data1,
	//	selfProjectGuid.Data2,
	//	selfProjectGuid.Data3,
	//	selfProjectGuid.Data4[0],
	//	selfProjectGuid.Data4[1],
	//	selfProjectGuid.Data4[2],
	//	selfProjectGuid.Data4[3],
	//	selfProjectGuid.Data4[4],
	//	selfProjectGuid.Data4[5],
	//	selfProjectGuid.Data4[6],
	//	selfProjectGuid.Data4[7]
	//);

	//plot = guidBuffer;

	return plot;
}
