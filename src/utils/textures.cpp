#include "textures.hpp"
#include "GameSettings.hpp" // For BIG, MEDIUM, SMALL enums
#include <iostream>

// Define the global texture objects
sf::Texture tPlayer, tAsteroid[3], tBullet, tUFO, tUFOBullet;
sf::Texture tBulletUp, tLifeUp, tDoubleBullet, tPenetratingBullet;
sf::Texture tBackground;

bool loadTextures() {
	std::string dir = "textures/";
	if (!tPlayer.loadFromFile(dir + "player.png") || !tAsteroid[BIG].loadFromFile(dir + "asteroidBig.png") ||
		!tAsteroid[MEDIUM].loadFromFile(dir + "asteroidMedium.png") ||
		!tAsteroid[SMALL].loadFromFile(dir + "asteroidSmall.png") || !tBullet.loadFromFile(dir + "bullet.png") ||
		!tUFO.loadFromFile(dir + "ufo.png") || !tUFOBullet.loadFromFile(dir + "ufoBullet.png") ||
		!tBulletUp.loadFromFile(dir + "biggerBullet.png") || !tLifeUp.loadFromFile(dir + "lifeUp.png") ||
		!tDoubleBullet.loadFromFile(dir + "doubleBullet.png") ||
		!tPenetratingBullet.loadFromFile(dir + "penetratingBullet.png") ||
		!tBackground.loadFromFile(dir + "background.png")) {
		std::cout << "Error loading textures\n";
		return false;
	}

	tPlayer.setSmooth(true);
	tAsteroid[BIG].setSmooth(true);
	tAsteroid[MEDIUM].setSmooth(true);
	tAsteroid[SMALL].setSmooth(true);
	tBullet.setSmooth(true);
	tUFO.setSmooth(true);
	tUFOBullet.setSmooth(true);

	tBulletUp.setSmooth(true);
	tLifeUp.setSmooth(true);
	tDoubleBullet.setSmooth(true);
	tPenetratingBullet.setSmooth(true);
	tBackground.setRepeated(true);
	return true;
}