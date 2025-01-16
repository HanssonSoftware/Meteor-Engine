/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

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


String ICommandlet::Search(String Name)
{
	Name = String::Format("-%s", Name.Chr());

	const size_t queueSize = currentQueue.size();
	for (size_t i = 0; i < queueSize; i++)
	{
		String Temp = currentQueue[i];

		wchar_t* g = const_cast<wchar_t*>(Temp.Chr());
		g = wcstok(0, L" ", &g);
		if (wcscmp(g, Name.Chr()) == 0)
		{
			return currentQueue[i];
		}
	}

	return "";
}

void ICommandlet::Parse(int Count, char* Array[])
{
	executableLocation = Array[0];
	currentQueue.resize(Count);

	// Create a background, if parameter is found 
	char* indexedParameterBefore = Array[0];
	for (int i = 1; i < Count; i++)
	{
		// Grab a temporary alias from the array
		char* indexedParameter = Array[i];
		
		if (i + 1 <= Count)
		{
			char* indexed2Parameter = Array[i + 1];
			if (indexed2Parameter == nullptr)
			{
				currentQueue.push_back(String::Format("%s 1", String(indexedParameter).Chr()));
				i++;
				continue;
			}

			if (strcmp(strtok(indexed2Parameter, "-"), indexed2Parameter) == 0)
			{
				currentQueue.push_back(String::Format("%s %s", String(indexedParameter).Chr(), String(indexed2Parameter).Chr()));
				i++;
				continue;
			}
		}

		currentQueue.push_back(indexedParameter);
	}
}

