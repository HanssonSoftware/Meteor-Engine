/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <vector>
#include <guiddef.h>

typedef struct ProjectSection
{

} ProjectSection;

typedef struct Project
{
	Project(String displayName, String projectNameWithExtension, const String ProjectTypeGUID, const String ProjectUniqueGUID);

	String projectName;

	String projectNamePath;

	String selfProjectGuid;

	String formatToOutput() const;

	std::vector<ProjectSection> projectSubs;

} Project;

struct Solution
{
	Solution(const String Name);

	std::vector<Project> projects;

	std::vector<String> libraries;

private:
	String mainProjectName;
};
