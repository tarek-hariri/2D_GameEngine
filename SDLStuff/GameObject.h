#pragma once

#include "Game.h"

class GameObject {
public:
	GameObject(std::string textureSheet, int initialX, int initialY, int spriteWidth, int spriteHeight);

	void Update(double deltaTime, Camera* camera);
	void Render();

	int getX();
	int getY();
	
	void setWorldX(float newX);
	void setWorldY(float newY);

	void setX(int newPos);
	void setY(int newPos);

	float getWorldX();
	float getWorldY();

	void setTexture(SDL_Texture* sprite);

private:
	int xPos, yPos;
	float worldX, worldY;

	int spriteHeight, spriteWidth;
	
	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
};