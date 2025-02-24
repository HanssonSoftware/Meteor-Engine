/* Copyright 2020 - 2025, Saxon Software. All rights reserved. */

#include "AssetManager.h"
#include <mutex>


std::mutex mtx;
AssetManager* AssetManager::Get()
{
    std::lock_guard<std::mutex> g(mtx);

    static AssetManager instance;
    return &instance;
}

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}
