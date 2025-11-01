/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/Array.h>


/** Simple aliased function. */
template<typename ...>
struct Delegate
{
	Delegate() = default;

	virtual ~Delegate() noexcept = default;

	typedef void(*function)(...);

	template<class T, void(T::*Func)(...)>
	void Bind(T* object, Func same)
	{

	}

	void Broadcast(...)
	{
		
	}

	Array<function> functions;
};


#define spawnDelegate(RetVal, Name, ...) typedef RetVal (*Name)(__VA_ARGS__)

#define CREATE_DELEGATE(Name, ...) spawnDelegate(void, Name, __VA_ARGS__);

#define CREATE_DELEGATE_RETVAL(RetVal, Name, ...) 

