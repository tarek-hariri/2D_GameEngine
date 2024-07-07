#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(std::string textureFile) {
	SDL_Surface* tempSurface = IMG_Load(textureFile.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return texture;

}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Game::renderer, texture, &src, &dest);
}
