/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <File/File.h>

class String;
struct Solution;

class SolutionGenerator
{
	friend struct Solution;
public:
	SolutionGenerator(const String Name, const String Location);

	SolutionGenerator(const String NameLoc);

	bool isGood() const
	{
		return newSolution.isValid();
	}

	FMFile* getHandle()
	{
		return &newSolution;
	}

	String getSolutionName() const
	{
		return solutionName;
	}

	void setSolutionName(const String newName)
	{
		solutionName = newName;
	};

	bool makeSolution();

	bool startSolutionFill();

	bool readSolution();

	bool makeProjects(const Solution Root);
private:
	String solutionName;

	FMFile newSolution;

	std::vector<FMFile*> projects;
};

