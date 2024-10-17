/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */

#pragma once
#include <vector>
#include <functional>

struct Delegate
{
	void add(std::function<void()> functionCallback) {
		callbacks.push_back(functionCallback);
	}

	void invoke()
	{
		for (std::function<void()> a : callbacks)
		{
			a();
		}
	}

private:
	std::vector<std::function<void()>> callbacks;
};
