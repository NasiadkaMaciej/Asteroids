#include <SFML/Graphics.hpp>
#include <iostream>

sf::Texture tPlayer, tAsteroid[3], tBullet;
sf::Texture tBulletUp, tLifeUp;

#define BIG 0
#define MEDIUM 1
#define SMALL 2

void loadTextures()
{
	std::string dir = "textures/";
	if (!tPlayer.loadFromFile(dir + "player.png") ||
		!tAsteroid[BIG].loadFromFile(dir + "asteroidBig.png") ||
		!tAsteroid[MEDIUM].loadFromFile(dir + "asteroidMedium.png") ||
		!tAsteroid[SMALL].loadFromFile(dir + "asteroidSmall.png") ||
		!tBullet.loadFromFile(dir + "bullet.png") ||
		!tBulletUp.loadFromFile(dir + "powerUp.png") ||
		!tLifeUp.loadFromFile(dir + "lifeUp.png"))
	{
		std::cout << "Error loading textures\n";
	}
	else
	{
		tPlayer.setSmooth(true);
		tAsteroid[BIG].setSmooth(true);
		tAsteroid[MEDIUM].setSmooth(true);
		tAsteroid[SMALL].setSmooth(true);
		tBullet.setSmooth(true);
		tBulletUp.setSmooth(true);
		tLifeUp.setSmooth(true);
	}
}

#define BULLET 0
#define POWER_BULLET 1

void setBullet(int size){
	std::string dir = "textures/", bullet;

	switch (size)
	{
	case 0:
		bullet = "bullet.png";
		break;
	case 1:
		bullet = "powerBullet.png";
		break;
	}
	tBullet.loadFromFile(dir + bullet);
}