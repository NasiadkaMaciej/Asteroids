#pragma once

#include "../control.hpp"
#include "../sounds.hpp"
#include "../textures.hpp"
#include "Entity.hpp"
#include <SFML/Audio.hpp>
#include <SFML/System/Angle.hpp>
#include <list>
#include <memory>

enum eTypes : char { _PLAYER, _ASTEROID, _BULLET, _UFO, _POWERUP };

// Multiplier that ensures equal speed of entities at all resolutions
float speedScale = gameSettings.resY / 200;
float asteroidSpeed[3] = { speedScale, speedScale * 1.5f, speedScale * 2 };
float asteroidDiffSpeed[3] = { asteroidSpeed[0] / 2, asteroidSpeed[1] / 2, asteroidSpeed[2] / 2 };

struct Scale {
	float x, y;
	Scale(float X, float Y) {
		x = X * screenScale;
		y = Y * screenScale;
	}
};

// min + rand() % ( max - min + 1 )
// returns random value excluding 0
int random(int range, int modifier) {
	int randomValue;
	do
		randomValue = std::rand() % range - modifier;
	while (randomValue == 0);
	return randomValue;
};