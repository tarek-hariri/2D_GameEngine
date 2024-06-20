#pragma once
#include "TERRAIN.h"

class Tile {
public:
	Tile(TERRAIN terrain) { this->terrain = terrain; }
	TERRAIN getTerrain() { return terrain; }

private:
	TERRAIN terrain;
};