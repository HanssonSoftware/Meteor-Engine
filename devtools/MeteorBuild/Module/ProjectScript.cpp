/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */


#include "ProjectScript.h"

bool ProjectScript::Finalize(String& output)
{
	output += "Microsoft Visual Studio Solution File, Format Version 12.00\n# Visual Studio Version 17\nVisualStudioVersion = 17.11.35327.3\nMinimumVisualStudioVersion = 10.0.40219.1";
	
	return false;
}
