#pragma once

#include <SFML/Audio.hpp>
#include <SFML/System/Angle.hpp>

// Forward declaration
class Entity;

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