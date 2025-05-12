/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Logging/LogMacros.h>
#include <Types/String.h>


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
	if (Temp.IsEmpty())
	{
		//if (strcmp(typeid(T).name(), typeid(bool).name()) == 0)  
			//return false;

		//if (strcmp(typeid(T).name(), typeid(String).name()) == 0)  
			//return "";
	}

	//if (strcmp(typeid(T).name(), typeid(bool).name()) == 0)
	//{
	//	return true;
	//}

	char* A = const_cast<char*>(Temp.Chr());
	char* B = strtok_s(A, " ", &A);

	return A;
};