/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Intermediate.h"
#include <Commandlet.h>

String Intermediate::GetIntermediateDirectory()
{
	const String directory = ICommandlet::Parse("-int");

	return directory.IsEmpty() ? String("") : directory;
}
