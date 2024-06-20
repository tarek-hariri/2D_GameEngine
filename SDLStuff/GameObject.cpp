#include "GameObject.h"
#include "TextureManager.h"
#include "Settings.h"

GameObject::GameObject(const char* spriteSheet, int initialX, int initialY, int spriteWidth, int spriteHeight) {
	objTexture = TextureManager::LoadTexture(spriteSheet);
	
	xPos, worldX = initialX;
	yPos, worldY = initialY;

	srcRect.h = spriteHeight * SCALE;
	srcRect.w = spriteWidth * SCALE;

	this->spriteHeight = spriteHeight;
	this->spriteWidth = spriteWidth;
}

void GameObject::Update(double deltaTime) {
	srcRect.x = 0;
	srcRect.y = 0;

	// Account for sprites bigger than tile size... we want those sprites to render from its position tile upwards, not downwards!
	destRect.x = worldX * SCALED_TILE_SIZE - ((spriteWidth * SCALE - SCALED_TILE_SIZE)/2); // Dividing by two centers sprite on tile
	destRect.y = worldY * SCALED_TILE_SIZE - (spriteHeight * SCALE - SCALED_TILE_SIZE + 1*SCALE);
	destRect.w = spriteWidth * SCALE;
	destRect.h = spriteHeight * SCALE;
}

void GameObject::Render() {
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}

int GameObject::getX(){
	return xPos;
}

int GameObject::getY(){
	return yPos;
}

void GameObject::setWorldX(float newX){
	this->worldX = newX;
}

void GameObject::setWorldY(float newY){
	this->worldY = newY;
}

void GameObject::setX(int newPos){
	xPos = newPos;
}

void GameObject::setY(int newPos){
	yPos = newPos;
}


