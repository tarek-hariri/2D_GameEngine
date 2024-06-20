#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
	Player(const char* textureSheet, int initialX, int initialY, TileMap* tileMap, int spriteWidth, int spriteHeight, std::unordered_map<uint32_t, sPlayerDescription>* queuedMovements, uint32_t nUniqueID);
	bool Move(int dx, int dy);
	void Update(double deltaTime);

	enum PLAYER_STATE {
		WALKING,
		STANDING
	};

private:
	void InitializeMove(int dx, int dy);
	void FinishMove();

	TileMap* tileMap;

	// Interpolation update vars
	int srcX, srcY;
	int destX, destY;
	double animTimer;
	double ANIM_TIME = .5f;

	PLAYER_STATE state;

	// End interpolation vars

	std::unordered_map<uint32_t, sPlayerDescription>* queuedMovements;
	uint32_t nUniqueID;

};