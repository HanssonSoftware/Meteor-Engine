/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <atomic>


class Semaphore
{
public:
	Semaphore();

	~Semaphore();

	void wait();

	void fire(); 

	void reset();
private:
	std::atomic<char> flag = '0';
};

