#pragma once

#include "Entity.hpp"
#include "utils.hpp"

class Asteroid : public Entity {
	float rotationSpeed; // degrees per second

	// enum AsteroidSize { SMALL, MEDIUM, BIG };
	eSizes size;

  public:
	char type() { return _ASTEROID; }
	Asteroid(float X, float Y, float X_SPEED, float Y_SPEED, sf::Texture* TEXTURE)
	  : Entity(X, Y, X_SPEED, Y_SPEED, sf::degrees(static_cast<float>(std::rand() % 360)), TEXTURE) {
		// Determine size based on texture
		if (TEXTURE == &tAsteroid[BIG])
			size = BIG;
		else if (TEXTURE == &tAsteroid[MEDIUM])
			size = MEDIUM;
		else
			size = SMALL;

		rotationSpeed = (std::rand() % 180 - 90); // -90 to +89 degrees per second
	};

	void update() {
		// Apply time-based rotation
		sprite.setOrigin(sf::Vector2f{ sprite.getLocalBounds().size.x / 2, sprite.getLocalBounds().size.y / 2 });
		float dt = delta->Move / 1000.0f; // Convert time delta to seconds

		// Convert angle to float, add rotation, and convert back
		float angleAsFloat = angle.asDegrees();
		angleAsFloat += rotationSpeed * dt; // Smooth rotation based on time

		// Keep angle in reasonable range to prevent floating point issues
		if (angleAsFloat > 360.0f) angleAsFloat -= 360.0f;
		if (angleAsFloat < 0.0f) angleAsFloat += 360.0f;

		// Set the angle back
		angle = sf::degrees(angleAsFloat);

		// Update sprite rotation
		sprite.setRotation(angle);

		x += x_speed * delta->Move / 10;
		y += y_speed * delta->Move / 10;
		if (x > gameSettings.resX)
			x = 0;
		else if (x < 0)
			x = gameSettings.resX;
		if (y > gameSettings.resY)
			y = 0;
		else if (y < 0)
			y = gameSettings.resY;
	}
	// Generates asteroid at random edge of the screen
	static Asteroid* generateBig() {
		int side = std::rand() % 4;
		float x, y;
		switch (side) {
		case 0:
			x = 0;
			y = rand() % gameSettings.resY;
			break;
		case 1:
			x = gameSettings.resX;
			y = rand() % gameSettings.resY;
			break;
		case 2:
			x = rand() % gameSettings.resX;
			y = 0;
			break;
		case 3:
			x = rand() % gameSettings.resX;
			y = gameSettings.resY;
			break;
		}
		return new Asteroid(x,
							y,
							random(asteroidSpeed[BIG], asteroidDiffSpeed[BIG]),
							random(asteroidSpeed[BIG], asteroidDiffSpeed[BIG]),
							&tAsteroid[BIG]);
	}
	static Asteroid* generate(float X, float Y, eSizes asteroidNum) {
		return new Asteroid(X,
							Y,
							random(asteroidSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
							random(asteroidSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
							&tAsteroid[asteroidNum]);
	}
};