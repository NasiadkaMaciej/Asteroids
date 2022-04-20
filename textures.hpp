#include <SFML/Graphics.hpp>
#include <iostream>
sf::Texture tPlayer, tAsteroid[3], tBullet;
sf::Texture tBulletUp, tLifeUp;

#define big 0
#define medium 1
#define small 2

std::string dir = "textures/";

void
loadTextures()
{
  if (!tPlayer.loadFromFile(dir + "player.png") ||
      !tAsteroid[big].loadFromFile(dir + "asteroidBig.png") ||
      !tAsteroid[medium].loadFromFile(dir + "asteroidMedium.png") ||
      !tAsteroid[small].loadFromFile(dir + "asteroidSmall.png") ||
      !tBullet.loadFromFile(dir + "bullet.png") ||
      !tBulletUp.loadFromFile(dir + "powerUp.png") ||
      !tLifeUp.loadFromFile(dir + "lifeUp.png"))
    std::cout << "Error loading textures\n";
  tPlayer.setSmooth(true);
  tAsteroid[big].setSmooth(true);
  tAsteroid[medium].setSmooth(true);
  tAsteroid[small].setSmooth(true);
  tBullet.setSmooth(true);

  tBulletUp.setSmooth(true);
  tLifeUp.setSmooth(true);
}