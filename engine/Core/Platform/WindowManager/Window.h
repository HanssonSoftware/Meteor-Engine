/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Vector.h>
#include "WindowExtender.h"
#include <vector>

struct WindowCreateInfo
{
	/** Constant ID, good for searching window.*/
	String windowID;

	/** Distinguish between the ID, the ID is for searching purposes.*/
	String windowName;

	Vector2<uint32> size = 0;

	int flags = -1;
};


class IWindow
{
public:
	friend class IWindowManager;

	IWindow(IWindowManager* newOwner);

	virtual ~IWindow() = default;

	Vector2<uint32> GetSize() const
	{
		return windowData.size;
	}

	virtual void DestroyWindow() = 0;

	virtual bool CreateNativeWindow(const WindowCreateInfo* windowData) = 0;

	virtual void ShowWindow() = 0;

	virtual void SetTitle(const String newName) = 0;

	virtual void HideWindow() = 0;

	virtual void DrawAttention() = 0;

	virtual void* GetWindowHandle() = 0;

	WindowCreateInfo windowData;
protected:
	IWindowManager* owner;

	std::vector<IWindowExtender*> extensions;

	void* handle = nullptr;
};

