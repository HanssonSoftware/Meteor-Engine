/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include <Types/Vector.h>


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
	friend class WindowManager;

public:
	IWindow(WindowManager* newOwner);

	virtual ~IWindow();

	Vector2<uint32> getSize() const;

	virtual void destroyWindow() = 0;

	virtual void showWindow() = 0;

	virtual void setTitle(const String newName) = 0;

	virtual void hideWindow() = 0;

	virtual bool drawAttention() = 0;

	virtual void* getWindowHandle() = 0;

	WindowCreateInfo windowData;
protected:
	WindowManager* owner;

	void* Handle = nullptr;
};

