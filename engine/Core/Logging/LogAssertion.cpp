/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "LogAssertion.h"
#include <cstdarg>
#include <stdio.h>

LogAssertion::LogAssertion(const char* inputFile, const int inputLine, const char* statement, const char* inputMessage, ...)
    : assertLocationInFile(inputFile)
    , assertLineInFile(inputLine)
    , assertStatement(statement)
{
    va_list a = 0;
    va_start(a, inputMessage);
    const int requiredAmount = vsnprintf(nullptr, 0, inputMessage, a);

    char* super = new char[requiredAmount + 1];
    vsnprintf(super, requiredAmount, inputMessage, a);
    va_end(a);

    assertMessage = super;
    delete[] super;
}