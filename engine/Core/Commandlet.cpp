/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Commandlet.h"


static ICommandlet instance;

ICommandlet& ICommandlet::Get()
{

	return instance;
}

void ICommandlet::InitCommandlet(char* Input[])
{
}

String ICommandlet::Search(String Name)
{
	Name = String::Format("-%s", Name.Chr());
	const size_t s = currentQueue.size();

	for (size_t i = 0; i < s; i++)
	{
		String Temp = currentQueue[i];

		char* g = const_cast<char*>(Temp.Chr());
		g = strtok(0, " ");
		if (strcmp(g, Name.Chr()) == 0)
		{
			return currentQueue[i];
		}
	}

	return "";
}

void ICommandlet::Parse(int Count, char* Array[])
{

}

