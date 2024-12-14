/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Log/Exception.h>
#include <functional>
#include <thread>

class SlowTask
{
public:
	template <typename F, class Obj>
	void connectToFunction(F&& function, Obj* object, ...);

	void kill();
private:
	std::thread worker;
};

template<typename F, class Obj>
inline void SlowTask::connectToFunction(F&& function, Obj* object, ...)
{
	if (!object)
		THROW_EXCEPTION("Invalid object! Cannot connect to desired function.");

	worker = std::thread(function, object);

	//if (worker.joinable())
	//	worker.join();

}
