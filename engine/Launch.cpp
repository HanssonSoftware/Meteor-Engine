/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#ifdef MR_DEBUG
#include <crtdbg.h>
#endif // MR_DEBUG

#include <Windows/Windows.h>
#pragma warning (disable : 28251)

extern "C" __declspec(dllexport) int LaunchApplication(int ArgumentCount, char* Arguments[]);

int main(int ArgumentCount, char* Arguments[])
{
    int Result = LaunchApplication(ArgumentCount, Arguments);
#ifdef MR_DEBUG
    _CrtDumpMemoryLeaks();
#endif // MR_DEBUG
    return Result;
}

// DO NOT MAKE STATIC!!
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    int Result = LaunchApplication(/* Unused --> */ 0, &lpCmdLine);
#ifdef MR_DEBUG
    _CrtDumpMemoryLeaks();
#endif // MR_DEBUG
    return Result;
}