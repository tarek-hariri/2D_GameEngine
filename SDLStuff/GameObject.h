#pragma once

#include "Game.h"

class GameObject {
public:
	GameObject(const char* textureSheet, int initialX, int initialY, int spriteWidth, int spriteHeight);
	~GameObject();

	void Update(double deltaTime);
	void Render();

	int getX();
	int getY();
	
	void setWorldX(float newX);
	void setWorldY(float newY);

	void setX(int newPos);
	void setY(int newPos);

private:
	int xPos, yPos;
	float worldX, worldY;

	int spriteHeight, spriteWidth;
	
	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
};