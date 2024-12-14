/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#pragma once
#include <Widgets/Visual.h>
#include <Types/String.h>

class TextBlock : public Visual
{
public:
	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	void setText(String text);

	String getText() const { return content; };
private:
	String content;
};

