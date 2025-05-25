#include "utils.hpp"
#include "../utils/GameSettings.hpp"

// Define the global variables
float speedScale = 1.0f;
float bulletSpeed = 1.0f;
float asteroidSpeed[3] = { 1.0f, 1.5f, 2.0f };
float asteroidDiffSpeed[3] = { 0.5f, 0.75f, 1.0f };

void initializeSpeedValues() {
	speedScale = gameSettings.resY / 200;
	bulletSpeed = speedScale * 2.5f;
	asteroidSpeed[0] = speedScale * 0.8f;
	asteroidSpeed[1] = speedScale * 1.0f;
	asteroidSpeed[2] = speedScale * 1.2f;
	asteroidDiffSpeed[0] = asteroidSpeed[0] / 2;
	asteroidDiffSpeed[1] = asteroidSpeed[1] / 2;
	asteroidDiffSpeed[2] = asteroidSpeed[2] / 2;
}

Scale::Scale(float X, float Y) {
	x = X * screenScale;
	y = Y * screenScale;
}

// min + rand() % ( max - min + 1 )
// returns random value excluding 0
int random(int range, int modifier) {
	if (range <= 1) return 1;
	int randomValue;
	do
		randomValue = (std::rand() % (range * 2)) - range;
	while (randomValue == 0);
	return randomValue;
}