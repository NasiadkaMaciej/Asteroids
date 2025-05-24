#pragma once

#include "../utils/GameSettings.hpp"
#include <SFML/Graphics.hpp>

class Entity // general class for all existing entities
{
  public:
	float x, y, x_speed, y_speed;
	sf::Angle angle;
	bool life = true;
	int lifes = 0;
	sf::Sprite sprite;

	Entity(float X, float Y, float X_SPEED, float Y_SPEED, sf::Angle ANGLE, sf::Texture* TEXTURE);
	const void draw(sf::RenderWindow& window);
	virtual void update();

	bool operator==(const sf::Texture* txt) const;
};