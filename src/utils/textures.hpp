#pragma once

#include <SFML/Graphics.hpp>

// Global texture objects
extern sf::Texture tPlayer, tAsteroid[3], tBullet, tUFO, tUFOBullet;
extern sf::Texture tBulletUp, tLifeUp, tDoubleBullet, tPenetratingBullet;
extern sf::Texture tBackground;

// Function declaration
bool loadTextures();