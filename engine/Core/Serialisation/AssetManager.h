/* Copyright 2020 - 2025, Hansson Software. All rights reserved. */

#pragma once
#include <Types/String.h>
#include "Importer.h"
//#include <Serialisation/FontImporter.h>


class AssetManager
{
public:
	static AssetManager* Get();

	template<class T>
	T* loadAsset(const String ID);

private:
	AssetManager();

	~AssetManager();
};

template<class T>
inline T* AssetManager::loadAsset(const String ID)
{


	return nullptr;
}
