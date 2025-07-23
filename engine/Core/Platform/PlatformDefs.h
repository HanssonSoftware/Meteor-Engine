/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */
#pragma once
#include <cstdint>
#include "PlatformLayout.h"

#ifdef MR_PLATFORM_WINDOWS
/** uint32 variable size is: 16bit */
typedef unsigned int uint32;
typedef unsigned long long uint64;
#else
/** uint32 variable size is: 16bit */
typedef unsigned int uint32;
typedef unsigned long long uint64;
#endif // _WIN64

typedef signed int sint32;
typedef unsigned short uint8;
typedef signed short sint8;
typedef signed long long sint64;