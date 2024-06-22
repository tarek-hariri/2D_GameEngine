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

Player::Player(const char* textureSheet, int initialX, int initialY, TileMap* tileMap, int spriteWidth, int spriteHeight, std::unordered_map<uint32_t, sPlayerDescription>* queuedMovements, uint32_t nUniqueID)
	: GameObject::GameObject(textureSheet, initialX, initialY, spriteWidth, spriteHeight){
	this->tileMap = tileMap;
	this->state = PLAYER_STATE::STANDING;
	this->queuedMovements = queuedMovements;
	this->nUniqueID = nUniqueID;
}

bool Player::Move(int dx, int dy)
{	
	// If a move is already being executed, don't move again
	if (state != PLAYER_STATE::STANDING) {
		return false;
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
		setWorldX(lerp(srcX, destX, animTimer / ANIM_TIME));
		setWorldY(lerp(srcY, destY, animTimer / ANIM_TIME));

		if (animTimer > ANIM_TIME) {
			FinishMove();

			// If a move is queued, execute it
			if (queuedMovements->find(nUniqueID) != queuedMovements->end()) {
				int moveX = (*queuedMovements)[nUniqueID].xPos - getX();
				int moveY = (*queuedMovements)[nUniqueID].yPos - getY();
				Move(moveX, moveY);
			}
		}
	}
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