/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "SolutionGenerator.h"
#include <Types/String.h>
#include <Log/Exception.h>
#include "Solution.h"

static constexpr const wchar_t* CPP_PROJECT_GUID = L"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}";

SolutionGenerator::SolutionGenerator(const String Name, const String Location)
{
	if (Name.isEmpty())
		THROW_EXCEPTION("SolutionGenerator (const String Name -> Empty or Null), You Must Provide a Name For The Solution!");

	String max = Location + Name;
	if (!Name.endsWith("sln"))
	{
		max = String::Format("%s.sln", Name.Chr());
	}

	solutionName = max;
	newSolution.Open(max, OPENRULE_WRITE, OVERRIDERULE_JUST_OPEN);
}

SolutionGenerator::SolutionGenerator(const String NameLoc)
{
	if (NameLoc.isEmpty())
		THROW_EXCEPTION("SolutionGenerator (const String NameLoc -> Empty or Null!), You Must Provide a Name/Location For The Solution!");

	String newName = NameLoc;
	if (!NameLoc.endsWith("sln"))
		newName = String::Format("%s.sln", NameLoc.Chr());

	solutionName = newName;
	newSolution.Open(newName, OPENRULE_WRITE, OVERRIDERULE_JUST_OPEN);
}

bool SolutionGenerator::makeSolution()
{
	if (!newSolution.isValid())
		THROW_EXCEPTION("Generated Solution File Somehow Invalid.");

	newSolution.Write("Microsoft Visual Studio Solution File, Format Version 12.00\n# Visual Studio Version 17\nVisualStudioVersion = 17.2.32505.173\nMinimumVisualStudioVersion = 10.0.40219.1\n");
	return true;
}

bool SolutionGenerator::startSolutionFill()
{
	makeSolution();

	return false;
}

bool SolutionGenerator::readSolution()
{
	if (!newSolution.isValid())
		THROW_EXCEPTION("Generated Solution File Somehow Invalid.");

	const String solutionName = String::Format("%s.%s", newSolution.getName().Chr(), newSolution.getExtension().Chr());

	Project(newSolution.getName(), solutionName, "", "");

	return false;
}

bool SolutionGenerator::makeProjects(const Solution Root)
{
	return false;
}
