/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
#include <Window/Window.h>

class WinWindow : public IWindow
{
public:
	virtual void destroyWindow() override;

	virtual void showWindow() override;

	virtual void setTitle(const String newName) override;

	virtual void hideWindow() override;

	virtual bool drawAttention() override;

	virtual void* getWindowHandle() override;
};

