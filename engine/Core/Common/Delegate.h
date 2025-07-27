/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once



/** Simple aliased function. */
struct Delegate
{

};


#define spawnDelegate(Name, ...) typedef void (*Name)(__VA_ARGS__)
#define spawnDelegateWRetVal(RetVal, Name, ...) typedef RetVal (*Name)(__VA_ARGS__)

#define CREATE_DELEGATE(Name, ...) spawnDelegate(Name, __VA_ARGS__);

#define CREATE_DELEGATE_RETVAL(RetVal, Name, ...) spawnDelegateWRetVal(RetVal, Name, __VA_ARGS__)

