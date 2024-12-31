/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Types/String.h>
#include <unordered_map>
#include <functional>
#include <Log/LogMacros.h>

LOG_ADDCATEGORY(Commandlet);

struct Commandlet
{
	String name;
};


class ICommandlet
{
public:
	static ICommandlet& Get();

	template <class T>
	static T& Get();

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
	String Temp = Search(Name);
	if (Temp.isEmpty())
	{
		return strcmp(typeid(T).name(), "bool") == 0 ? false : "";
	}

	if (strcmp(typeid(T).name(), "bool") == 0)
	{
		return true;
	}

	wchar_t* A = const_cast<wchar_t*>(Temp.Chr());
	wchar_t* B = wcstok(0, L" ", &A);

	return A;
};