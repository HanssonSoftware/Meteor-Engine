/* Copyright 2020 - 2024, Saxon Software. All rights reserved. */

#include "Object.h"

Object::Object()
{
	int ia = 0;
}

void Object::Frame(float deltaTime)
{
	if (!bCanBeTicked)
		return;

}

void Object::Init()
{
}

void Object::Destroy()
{
	delete this;
}

void Object::setCanTick(bool Has)
{
	bCanBeTicked = Has;
}
