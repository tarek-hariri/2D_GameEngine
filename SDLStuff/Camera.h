#pragma once
#include "Settings.h"

class Camera {
public:
	float getCameraX() {
		return cameraX;
	}

	float getCameraY() {
		return cameraY;
	}

	float getWorldStartX() {
		return DISPLAY_WIDTH / 2 - cameraX * SCALED_TILE_SIZE;
	}

	float getWorldStartY() {
		return DISPLAY_HEIGHT / 2 - cameraY * SCALED_TILE_SIZE;
	}

	void update(float newCamX, float newCamY) {
		this->cameraX = newCamX;
		this->cameraY = newCamY;
	}

private:
	float cameraX = 0;
	float cameraY = 0;
};