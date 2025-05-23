#pragma once

#include "Entity.hpp"
#include "utils.hpp"

class Asteroid : public Entity {
	float rotationSpeed; // degrees per second

	// enum AsteroidSize { SMALL, MEDIUM, BIG };
	eSizes size;

  public:
	char type() override;
	Asteroid(float X, float Y, float X_SPEED, float Y_SPEED, sf::Texture* TEXTURE);
	void update() override;

	// Generates asteroid at random edge of the screen
	static Asteroid* generateBig();
	static Asteroid* generate(float X, float Y, eSizes asteroidNum);
};