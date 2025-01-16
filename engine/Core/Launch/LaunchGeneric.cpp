/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#ifdef MR_DEBUG
#include <crtdbg.h>
#endif // MR_DEBUG

extern int launchStranger(int ArgumentCount, char* Arguments[]);

int main(int ArgumentCount, char* Arguments[])
{
    int Result = launchStranger(ArgumentCount, Arguments);
#ifdef MR_DEBUG
    _CrtDumpMemoryLeaks();
#endif // MR_DEBUG
    return Result;
}
