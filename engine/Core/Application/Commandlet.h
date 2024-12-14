/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <unordered_map>
#include <functional>
#include <Log/LogMacros.h>

LOG_ADDCATEGORY(Commandlet);

struct Commandlet
{
public:
	Commandlet() = default;

	Commandlet(const String Identifier, const String defaultValue, bool flag);

	Commandlet operator=(const Commandlet&);

	~Commandlet();

	bool getIsFlag() const { return bIsFlag; };

	void setIsFlag(bool NewValue) { bIsFlag = NewValue; };	
	
	String getName() const { return name; };

	void setName(const String NewValue) { name = NewValue; };	

	String getValue() const { return value; };

	void setValue(const String NewValue) { value = NewValue; };

private:
	String name;

	bool bIsFlag = false;

	String value;
};

class ICommandlet
{
public:
	static ICommandlet& Get();

	template <class T>
	static T& Get();

	virtual ~ICommandlet() = default;

	bool Except(String Name);

	void Parse(int Count, char* Array[]);

	bool addCommand(Commandlet Value);
private:
	std::vector<Commandlet> registeredCommands;

	std::vector<Commandlet> currentQueue;
};

