/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Commandlet.h"


ICommandlet& ICommandlet::Get()
{
	static ICommandlet instance;
	return instance;
}

template <class T>
T& ICommandlet::Get()
{
	static_assert(std::is_base_of<ICommandlet, T>::value);
	static T instance;
	return instance;
}

bool ICommandlet::Except(String Name)
{
	//for (Command& A : currentQueue)
	//{
	//	if (A.name == Name)
	//	{
	//		return true;
	//	}
	//}

	return false;
}

void ICommandlet::Parse(int Count, char* Array[])
{
	// Create a background, if parameter is found 
	char* indexedParameterBefore = Array[0];
	for (int i = 1; i < Count; i++)
	{
		// Grab a temporary alias from the array
		char* indexedParameter = Array[i];
		Commandlet data;

		// Detect if the alias is a parameter by delimiter "-"
		if (*indexedParameter == '-')
		{
			// Tokenize by removing the dash (-token -> token)
			indexedParameter = strtok(indexedParameter, "-");
			
			// Get the available commands array's size, and begin looping over it
			const size_t& commandsSize = registeredCommands.size();
			for (int j = 0; j < commandsSize; j++)
			{
				// Grab the indexed registered command, if it has the name, 
				// indexedParameterBefore will be set and parse the next index (i)
				const Commandlet& indexed = registeredCommands[j];
				if (indexed.getName() == indexedParameter)
				{
					indexedParameterBefore = indexedParameter;
					data.setName(indexedParameter);
					data.setIsFlag(indexed.getIsFlag());

					break;
				}
			}


			const size_t size = mbstowcs(0, indexedParameter, 0);
			wchar_t* buffer = (wchar_t*)mrmalloc((size + 1) * sizeof(wchar_t));
			mbstowcs(buffer, indexedParameter, size + 1);

			MR_LOG(LogCommandlet, Error, TEXT("No command found with: -%s"), buffer);
			mrfree(buffer);
		}
		// It is a parameter's value, we expect the last parsed parameter need it?
		else
		{
			data.setValue(indexedParameter);
		}

		if (data.getIsFlag())
		{

		}
	}
}

bool ICommandlet::addCommand(Commandlet Value)
{
	registeredCommands.push_back(Value);

	return false;
}

Commandlet::Commandlet(const String Identifier, const String defaultValue, bool flag)
	: name(Identifier)
	, value(defaultValue)
	, bIsFlag(flag)
{
	ICommandlet::Get().addCommand(*this);
}

Commandlet Commandlet::operator=(const Commandlet& other)
{
	return Commandlet(other.getName(), other.getValue(), other.getIsFlag());
}

Commandlet::~Commandlet()
{
}
