/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once

#ifdef MR_EDITORAPP_ENABLE

#ifdef _WIN64
#define MR_EDITORAPPLICATION_API __declspec(dllexport)
#endif // _WIN64
#else
#ifdef _WIN64
#define MR_EDITORAPPLICATION_API
#endif // _WIN64
#endif // MR_CORE_ENABLED
