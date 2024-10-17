/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#include "File.h"
#include <string>
#include <Windows.h>


File::~File()
{

}

File::File(const String& Name)
{
	fileHandle = nullptr;
}
