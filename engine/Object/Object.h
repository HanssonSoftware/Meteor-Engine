/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */


#pragma once

class Object
{
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

