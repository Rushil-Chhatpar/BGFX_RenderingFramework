#pragma once
#include "../../Game/Source/DataTypes.h"

class HeightMapComponent;

class HeightMapManager
{
public:
    HeightMapManager();
    ~HeightMapManager();

    void AddHeightMap(HeightMapComponent* heightMap)            { m_HeightMaps.push_back(heightMap); }

    std::vector<HeightMapComponent*> m_HeightMaps;
};