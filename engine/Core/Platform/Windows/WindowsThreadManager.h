/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Thread/ThreadManager.h>
#include <Thread/ThreadDescriptor.h>

class WindowsThread;

class WindowsThreadManager : public IThreadManager
{
public:
	static void Init();

	static void Shutdown();

	static WindowsThread* CreateThread(const ThreadInitializationDescriptor* descriptor);
};

using ThreadManager = WindowsThreadManager;