/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once

typedef struct Math
{
	template<typename A = float, typename B = float, typename C = float>
	constexpr C pith(A a, B b);

} Math;

typedef struct Storage
{

} Storage;

template<typename A, typename B, typename C>
constexpr inline C Math::pith(A a, B b)
{
	return a * a + b * b;
}
