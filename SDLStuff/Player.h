#pragma once

#include "GameObject.h"
#include "AnimationSet.h"

class Player : public GameObject {
public:

	Player(std::string textureSheet, int initialX, int initialY, TileMap* tileMap, int spriteWidth, int spriteHeight, std::unordered_map<uint32_t, sPlayerDescription>* queuedMovements, uint32_t nUniqueID, std::string animSetDir);
	bool Move(int dx, int dy);
	void Update(double deltaTime, Camera* camera);
	SDL_Texture* getSprite();

	enum PLAYER_STATE {
		WALKING,
		STANDING
	};

	bool getExecutedQueuedMove();
	void resetExecutedQueuedMove();

private:
	void InitializeMove(int dx, int dy);
	void FinishMove();

	TileMap* tileMap;


	// Animation vars
	float walkTimer;
	DIRECTION facing;

	AnimationSet animations;

	// End animation vars

	// Interpolation update vars
	int srcX, srcY;
	int destX, destY;
	double animTimer;
	double frameTimer;
	double ANIM_TIME = .3f;
	


	PLAYER_STATE state;
	// End interpolation vars

	std::unordered_map<uint32_t, sPlayerDescription>* queuedMovements;
	uint32_t nUniqueID;

	bool executedQueuedMove = false;
	int queuedDx = -1;
	int queuedDy = -1;

};