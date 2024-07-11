#include "GameObject.h"
#include "TextureManager.h"
#include "Settings.h"

GameObject::GameObject(std::string spriteSheet, int initialX, int initialY, int spriteWidth, int spriteHeight) {
	objTexture = TextureManager::LoadTexture(spriteSheet);
	
	xPos = initialX;
	worldX = initialX;
	yPos = initialY;
	worldY = initialY;
	
	srcRect.h = spriteHeight * SCALE;
	srcRect.w = spriteWidth * SCALE;

	this->spriteHeight = spriteHeight;
	this->spriteWidth = spriteWidth;
}

void GameObject::Update(double deltaTime, Camera* camera) {
	srcRect.x = 0;
	srcRect.y = 0;

	// Account for sprites bigger than tile size... we want those sprites to render from its position tile upwards, not downwards!
	destRect.x = camera->getWorldStartX() + worldX * SCALED_TILE_SIZE - ((spriteWidth * SCALE - SCALED_TILE_SIZE) / 2); // Dividing by two centers sprite on tile
	destRect.y = camera->getWorldStartY() + worldY * SCALED_TILE_SIZE - (spriteHeight * SCALE - SCALED_TILE_SIZE + 1*SCALE);
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

float GameObject::getWorldX(){
	return worldX;
}

float GameObject::getWorldY(){
	return worldY;
}

void GameObject::setTexture(SDL_Texture* sprite) {
	// TODO: Need to somehow dereference the initial sprite. Could add bool when making GameObject indicating if its a Player or something that will need to eventually deallocate here
	this->objTexture = sprite;
}


