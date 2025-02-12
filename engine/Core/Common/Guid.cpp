/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "Guid.h"



Guid::Guid()
{
	if (SUCCEEDED(CoCreateGuid(&id)))
	{

	}
}

Guid::~Guid()
{
}
