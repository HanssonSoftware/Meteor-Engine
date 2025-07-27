/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#include "SystemLayer.h"

SystemLayer::SystemLayer(const String Name) noexcept
    : Layer(Name)
{

}

String SystemLayer::GetError()
{
    return "???";
}

void SystemLayer::Init()
{
}

void SystemLayer::Attached()
{
}

void SystemLayer::Removed()
{
}

void SystemLayer::Update()
{
}
