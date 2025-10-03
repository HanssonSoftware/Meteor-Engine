/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Array.h>
#include <Types/String.h>
#include "SolutionDescriptor.h"

class IFile;

class Solution
{
public:
	Solution() = default;

	virtual ~Solution() noexcept = default;

	void SetFileHandle(IFile* newHandle) { file = newHandle; }

	IFile* GetFileHandle() const { return file; }
protected:
	Array<ProjectDefine*> projects;

	IFile* file = nullptr;

	String projectName;
};

/*

Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 17
VisualStudioVersion = 17.11.35327.3
MinimumVisualStudioVersion = 10.0.40219.1
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Meteor", "meteor.vcxproj", "{709B7964-F338-40B8-B8E2-A2898B438A1C}"
EndProject
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Editor", "editor\Editor.vcxproj", "{13CDFF27-4E0F-4C92-92EC-F6D925CD8D09}"
	ProjectSection(ProjectDependencies) = postProject
		{709B7964-F338-40B8-B8E2-A2898B438A1C} = {709B7964-F338-40B8-B8E2-A2898B438A1C}
	EndProjectSection
EndProject
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "MeteorBuild", "devtools\MeteorBuild\MeteorBuild.vcxproj", "{E75C5354-0747-4518-9E9B-B4FDFF214314}"
EndProject
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Renderer", "engine\Renderer\Renderer.vcxproj", "{244D5E70-1DF7-4426-B141-C2059F72BF7F}"
EndProject
Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "Modules", "Modules", "{02EA681E-C7D8-13C7-8484-4AC65E1B71E8}"
EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|x64 = Debug|x64
		Shipping|x64 = Shipping|x64
	EndGlobalSection
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
		{244D5E70-1DF7-4426-B141-C2059F72BF7F}.Debug|x64.ActiveCfg = Debug|x64
		{244D5E70-1DF7-4426-B141-C2059F72BF7F}.Debug|x64.Build.0 = Debug|x64
		{244D5E70-1DF7-4426-B141-C2059F72BF7F}.Shipping|x64.ActiveCfg = Release|x64
		{244D5E70-1DF7-4426-B141-C2059F72BF7F}.Shipping|x64.Build.0 = Release|x64
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	GlobalSection(NestedProjects) = preSolution
		{244D5E70-1DF7-4426-B141-C2059F72BF7F} = {02EA681E-C7D8-13C7-8484-4AC65E1B71E8}
	EndGlobalSection
	GlobalSection(ExtensibilityGlobals) = postSolution
		SolutionGuid = {7442A397-EEB5-400C-B726-B1A2CAA77F66}
	EndGlobalSection
EndGlobal
*/
