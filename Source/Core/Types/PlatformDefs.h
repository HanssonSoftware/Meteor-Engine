/* Copyright 2020 - 2024, Fenix Software. All rights reserved. */
#pragma once
#include <cstdint>


#ifdef _WIN32
/** uint32 variable size is: 16bit */
typedef unsigned int uint32;
typedef unsigned long long uint64;
#else
/** uint32 variable size is: 16bit */
typedef unsigned int uint32;
typedef unsigned long long uint64;
#endif // _WIN32

typedef signed int sint32;
typedef signed long long sint64;