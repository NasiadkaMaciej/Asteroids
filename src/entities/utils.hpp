#pragma once

#include "../utils/GameSettings.hpp"
#include "../utils/control.hpp"
#include "../utils/sounds.hpp"
#include "../utils/textures.hpp"
#include <SFML/Audio.hpp>
#include <SFML/System/Angle.hpp>
#include <list>
#include <memory>

// Forward declaration
class Entity;

enum eTypes : char { _PLAYER, _ASTEROID, _BULLET, _UFO, _POWERUP };

// Multiplier that ensures equal speed of entities at all resolutions
extern float speedScale;
extern float bulletSpeed;
extern float asteroidSpeed[3];
extern float asteroidDiffSpeed[3];

struct Scale {
	float x, y;
	Scale(float X, float Y);
};

void initializeSpeedValues();

int random(int range, int modifier);