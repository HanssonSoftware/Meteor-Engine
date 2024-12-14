/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Semaphore.h"
#include <thread>

Semaphore::Semaphore()
{
	
}

Semaphore::~Semaphore()
{
}

void Semaphore::wait()
{
	while (flag != '1')
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void Semaphore::reset()
{
	flag = '0';
}

void Semaphore::fire()
{
	flag = '1';
}
