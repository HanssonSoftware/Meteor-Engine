/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "Timer.h"
#include <Types/String.h>
#include <type_traits>

static_assert(!std::is_same_v<Timer, ITimer>, "Timer is base type!");

Time ITimer::Now() noexcept
{
	return {};
}

String ITimer::Format(const String& formatting)
{
	return String();
}