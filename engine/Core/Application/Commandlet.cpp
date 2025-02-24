/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Commandlet.h"
#include <Log/Exception.h>


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
	const size_t s = currentQueue.size();

	for (size_t i = 0; i < s; i++)
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

	// Create a background, if parameter is found 
	char* indexedParameterBefore = Array[0];
	for (int i = 1; i < Count; i++)
	{
		char* parameter = Array[i];
		if (i + 1 <= Count)
		{
			char* parameterSecond = Array[i + 1];

			if (parameterSecond != nullptr)
			{
				if (parameterSecond[0] == '-')
				{
					currentQueue.push_back(parameter);
					continue;
				}
				else
				{
					currentQueue.push_back(String::Format("%s %s", String(parameter).Chr(), String(parameterSecond).Chr()));

					i++;
					continue;
				}
			}
			else
			{
				currentQueue.push_back(String::Format("%s %s", String(parameter).Chr(), L"1"));

				i++;
				continue;
			}
		}
	}
}

