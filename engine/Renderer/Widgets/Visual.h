/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Object/Object.h>
#include <Platform/PlatformDefs.h>
#include <Types/Vector.h>

typedef enum EVisibleState
{
	Collapsed,
	Hidden,
	Visible

} EVisibleState;

class Visual : public Object
{
public:
	void createWidget(Vector2<uint32> size);

	virtual void Init();

	virtual void Update(float deltaTime);

	EVisibleState getVisibility() const { return visibility; };

	void getVisibility(EVisibleState NewState) { visibility = NewState; };
private:
	Vector2<float> screenPos;

	Vector2<float> screenSize;

	EVisibleState visibility = Visible;
};

