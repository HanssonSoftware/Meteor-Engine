/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#pragma once
#include <type_traits>


class Subsystem
{
public:
	friend class CoreSubsystem;

	Subsystem(const Subsystem&) = delete;

	Subsystem() = default;

	virtual ~Subsystem() = default;
protected:
	virtual void tick(float deltaTime);

private:
};

template<typename C>
inline C& getSubsystem()
{
	static_assert(std::is_base_of<Subsystem, C>::value, "C does not inherit from Subsystem class!");
	static C instance;
	return instance;
}