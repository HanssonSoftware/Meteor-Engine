/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Logging/LogMacros.h>
#include <Types/String.h>
#include <vector>

LOG_ADDCATEGORY(Commandlet);

struct Commandlet
{
	String name;
};


class ICommandlet
{
public:
	static ICommandlet& Get();

	static void InitCommandlet(char* Input[]);

	virtual ~ICommandlet() = default;

	String Search(String Name);

	template<typename T = String>
	T Expected(const String Name);

	void Parse(int Count, char* Array[]);

	String executableLocation;
private:
	std::vector<String> registeredCommands;

	std::vector<String> currentQueue;
};

template<typename T>
T ICommandlet::Expected(const String Name)
{

	return 0;
};