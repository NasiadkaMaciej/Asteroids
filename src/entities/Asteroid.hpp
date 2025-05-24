#pragma once

#include "Entity.hpp"
#include "utils.hpp"

class Asteroid : public Entity {
	float rotationSpeed; // degrees per second

	// enum AsteroidSize { SMALL, MEDIUM, BIG };
	eSizes size;

  public:
	Asteroid(float X, float Y, float X_SPEED, float Y_SPEED, sf::Texture* TEXTURE);
	void update() override;

	// Generates asteroid at random edge of the screen
	static std::unique_ptr<Asteroid> generateBig();
	static std::unique_ptr<Asteroid> generate(float X, float Y, eSizes asteroidNum);
};