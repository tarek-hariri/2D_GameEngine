#include "Player.h"

// f is [0,1] that describes the progress of the movement between a and b
double lerp(double a, double b, double f)
{
	double output = a * (1.0 - f) + (b * f);

	if (b > a && output > b){
		return b;
	}
	if (b < a && output < b) {
		return b;
	}
	return output;
}

Player::Player(std::string textureSheet, int initialX, int initialY, TileMap* tileMap, int spriteWidth, int spriteHeight, std::unordered_map<uint32_t, sPlayerDescription>* queuedMovements, uint32_t nUniqueID, std::string animSetDir)
	: GameObject::GameObject(textureSheet, initialX, initialY, spriteWidth, spriteHeight) {
	this->tileMap = tileMap;
	this->state = PLAYER_STATE::STANDING;
	this->queuedMovements = queuedMovements;
	this->nUniqueID = nUniqueID;
	this->facing = DIRECTION::SOUTH;


	// Load walking textures
	SDL_Texture* northwalk_0 = TextureManager::LoadTexture(animSetDir + "NORTHWALK_0.png");
	SDL_Texture* northwalk_1 = TextureManager::LoadTexture(animSetDir + "NORTHWALK_1.png");
	SDL_Texture* northwalk_2 = TextureManager::LoadTexture(animSetDir + "NORTHWALK_2.png");
	SDL_Texture* southwalk_0 = TextureManager::LoadTexture(animSetDir + "SOUTHWALK_0.png");
	SDL_Texture* southwalk_1 = TextureManager::LoadTexture(animSetDir + "SOUTHWALK_1.png");
	SDL_Texture* southwalk_2 = TextureManager::LoadTexture(animSetDir + "SOUTHWALK_2.png");
	SDL_Texture* eastwalk_0 = TextureManager::LoadTexture(animSetDir + "EASTWALK_0.png");
	SDL_Texture* eastwalk_1 = TextureManager::LoadTexture(animSetDir + "EASTWALK_1.png");
	SDL_Texture* eastwalk_2 = TextureManager::LoadTexture(animSetDir + "EASTWALK_2.png");
	SDL_Texture* westwalk_0 = TextureManager::LoadTexture(animSetDir + "WESTWALK_0.png");
	SDL_Texture* westwalk_1 = TextureManager::LoadTexture(animSetDir + "WESTWALK_1.png");
	SDL_Texture* westwalk_2 = TextureManager::LoadTexture(animSetDir + "WESTWALK_2.png");


	// Load standing textures
	SDL_Texture* northstand = TextureManager::LoadTexture(animSetDir + "NORTHWALK_1.png");
	SDL_Texture* southstand = TextureManager::LoadTexture(animSetDir + "SOUTHWALK_1.png");
	SDL_Texture* eaststand = TextureManager::LoadTexture(animSetDir + "EASTWALK_1.png");
	SDL_Texture* weststand = TextureManager::LoadTexture(animSetDir + "WESTWALK_1.png");

	std::vector<SDL_Texture*> northwalk = { northwalk_0, northwalk_1, northwalk_2 };
	std::vector<SDL_Texture*> southwalk = { southwalk_0, southwalk_1, southwalk_2 };
	std::vector<SDL_Texture*> eastwalk = { eastwalk_0, eastwalk_1, eastwalk_2 };
	std::vector<SDL_Texture*> westwalk = { westwalk_0, westwalk_1, westwalk_2 };

	this->animations = AnimationSet(northwalk, eastwalk, southwalk, westwalk, northstand, eaststand, southstand, weststand);



}

bool Player::Move(int dx, int dy)
{	
	// If a move is already being executed, don't move again
	if (state != PLAYER_STATE::STANDING) {
		return false;
	}

	// Direction detection
	if (dx == 1 && dy == 0) {
		this->facing = DIRECTION::EAST;
	}
	else if( dx == -1 && dy == 0){
		this->facing = DIRECTION::WEST;
	}
	else if (dx == 0 && dy == 1) {
		this->facing = DIRECTION::SOUTH;
	}
	else if (dx == 0 && dy == -1) {
		this->facing = DIRECTION::NORTH;
	}

	int newX= getX() + dx;
	int newY = getY() + dy;


	// Bounds checking 
	// TODO: consolidate checks (may not be desirable if diagonal motion implemented)
	if (!(newX > -1 && newX < tileMap->getWidth())) {
		return false;
	}
	if (!(newY > -1 && newY < tileMap->getHeight())) {
		return false;
	}

	InitializeMove(dx, dy);

	setX(newX);
	setY(newY);

	return true;
}

void Player::Update(double deltaTime, Camera* camera) {
	if (state == PLAYER_STATE::WALKING) {
		animTimer += deltaTime/1000;
		walkTimer += deltaTime/1000;
		frameTimer += deltaTime/1000;
		setWorldX(lerp(srcX, destX, animTimer / ANIM_TIME));
		setWorldY(lerp(srcY, destY, animTimer / ANIM_TIME));

		if (animTimer > ANIM_TIME) {
			walkTimer -= (animTimer - ANIM_TIME);
			FinishMove();

			// If a move is queued, execute it
			if (queuedMovements->find(nUniqueID) != queuedMovements->end()) {
				int moveX = (*queuedMovements)[nUniqueID].xPos - getX();
				int moveY = (*queuedMovements)[nUniqueID].yPos - getY();
				Move(moveX, moveY);
			}
			else
				walkTimer = 0;
		}
	}
	// update texture to be rendered
	GameObject::setTexture(this->getSprite());

	GameObject::Update(deltaTime, camera);
}

void Player::InitializeMove(int dx, int dy){
	this->srcX = getX();
	this->srcY = getY();
	this->destX = getX() + dx;
	this->destY = getY() + dy;
	animTimer = 0;
	state = PLAYER_STATE::WALKING;
}

void Player::FinishMove() {
	state = PLAYER_STATE::STANDING;
	this->setWorldX(destX);
	this->setWorldY(destY);
	this->srcX = 0;
	this->srcY = 0;
	this->destX = 0;
	this->destY = 0;

}

SDL_Texture* Player::getSprite() {
	if (this->state == PLAYER_STATE::STANDING) {
		return animations.getStanding(this->facing);
	}
	else if (this->state == PLAYER_STATE::WALKING) {
		bool nextFrame = false;
		if (frameTimer > ANIM_TIME/2) {
			frameTimer = 0;
			nextFrame = true;
		}

		return animations.getWalking(this->facing, nextFrame);
	}
	else 
		return nullptr;
}