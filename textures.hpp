#include <SFML/Graphics.hpp>

sf::Texture tPlayer, tAsteroid, tAsteroidMedium, tAsteroidSmall, tBullet;

void loadTextures(){

sf::Texture xd;
tPlayer.loadFromFile("player.png");
tAsteroid.loadFromFile("asteroidBig.png");
tAsteroidMedium.loadFromFile("asteroidMedium.png");
tAsteroidSmall.loadFromFile("asteroidSmall.png");
tBullet.loadFromFile("bullet.png");
tPlayer.setSmooth(true);
tAsteroid.setSmooth(true);
tAsteroidMedium.setSmooth(true);
tAsteroidSmall.setSmooth(true);
tBullet.setSmooth(true);
}