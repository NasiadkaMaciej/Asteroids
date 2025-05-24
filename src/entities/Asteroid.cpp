#include "Asteroid.hpp"
#include "../utils/textures.hpp"

Asteroid::Asteroid(float X, float Y, float X_SPEED, float Y_SPEED, sf::Texture* TEXTURE)
  : Entity(X, Y, X_SPEED, Y_SPEED, sf::degrees(static_cast<float>(std::rand() % 360)), TEXTURE) {
	// Determine size based on texture
	if (TEXTURE == &tAsteroid[BIG])
		size = BIG;
	else if (TEXTURE == &tAsteroid[MEDIUM])
		size = MEDIUM;
	else
		size = SMALL;

	rotationSpeed = (std::rand() % 180 - 90); // -90 to +89 degrees per second
}

void Asteroid::update() {
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

std::unique_ptr<Asteroid> Asteroid::generateBig() {
	// Create asteroid at random edge of the screen
	int posX, posY;
	int edge = rand() % 4; // 0: top, 1: right, 2: bottom, 3: left

	switch (edge) {
	case 0: // Top edge
		posX = rand() % gameSettings.resX;
		posY = 0;
		break;
	case 1: // Right edge
		posX = gameSettings.resX;
		posY = rand() % gameSettings.resY;
		break;
	case 2: // Bottom edge
		posX = rand() % gameSettings.resX;
		posY = gameSettings.resY;
		break;
	case 3: // Left edge
		posX = 0;
		posY = rand() % gameSettings.resY;
		break;
	}

	return std::make_unique<Asteroid>(posX,
									  posY,
									  random(asteroidSpeed[BIG], asteroidDiffSpeed[BIG]),
									  random(asteroidSpeed[BIG], asteroidDiffSpeed[BIG]),
									  &tAsteroid[BIG]);
}

std::unique_ptr<Asteroid> Asteroid::generate(float X, float Y, eSizes asteroidNum) {
	return std::make_unique<Asteroid>(X,
									  Y,
									  random(asteroidSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
									  random(asteroidSpeed[asteroidNum], asteroidDiffSpeed[asteroidNum]),
									  &tAsteroid[asteroidNum]);
}