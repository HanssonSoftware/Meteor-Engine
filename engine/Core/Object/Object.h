/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#pragma once
//#include <DirectXMath.h>

class Object
{
	friend class SceneGraph;
public:
	Object();
	
	virtual ~Object() = default;

	virtual void Frame(float deltaTime);

	virtual void Init();

	virtual void Destroy();

	void setCanTick(bool Has);

	bool getCanTick() const
	{
		return bCanBeTicked;
	}

private:

	bool bCanBeTicked = true;
};

